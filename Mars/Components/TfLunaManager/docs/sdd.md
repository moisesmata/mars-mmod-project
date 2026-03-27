# TfLunaManager SDD

## 1. Purpose

`Mars::TfLunaManager` is the manager layer for TF-Luna LiDAR integration in this project's
Application -> Manager -> Driver architecture.

- **Application layer:** `Mars::MarsAnalyzer` (mission-specific commands and analysis)
- **Manager layer:** `Mars::TfLunaManager` (protocol handling, parser state, and driver bridging)
- **Driver layer:** `Drv::LinuxUartDriver` (UART bytes in/out and buffer ownership)

The manager isolates TF-Luna framing and protocol concerns from mission logic.

## 2. Requirements and Responsibilities

`TfLunaManager` is responsible for:

1. Accepting control actions from application (`START`, `STOP`, `RESET_PARSER`).
2. Receiving byte-stream data from the UART driver.
3. Returning received buffers back to the driver chain.
4. Parsing TF-Luna UART frames (`0x59 0x59`, 9-byte frame length).
5. Validating frame checksum.
6. Publishing decoded measurements to telemetry and to the application layer.
7. Publishing health and error counters/events for observability.

`TfLunaManager` is not responsible for mission-level MMOD decisions.

## 3. External Interfaces

### 3.1 Application-facing ports

- `controlIn: Mars.TfLunaControl` (sync input)
  - Consumes `TfLunaControlAction` from application layer.
- `frameOut: Mars.TfLunaFrame` (output)
  - Emits decoded `(distanceCm, signalStrength, temperatureCentiC)` tuples.

### 3.2 Driver-facing ports

- `lidarDriverReady: Drv.ByteStreamReady` (sync input)
  - Logs that the driver is ready.
- `lidarDataIn: Drv.ByteStreamData` (sync input)
  - Receives `(Fw::Buffer, Drv::ByteStreamStatus)` from the UART driver.
- `lidarDataReturnOut: Fw.BufferSend` (output)
  - Returns ownership of every received buffer after processing.

## 4. Data and State Model

Internal state variables:

- `m_enabled`: acquisition enabled flag controlled by `START` / `STOP`.
- `m_frame[9]` and `m_frameFill`: current parser frame buffer and fill level.
- `m_frameCount`: valid decoded frames.
- `m_checksumErrorCount`: checksum failures.
- `m_headerErrorCount`: header synchronization failures.
- `m_driverDropCount`: driver receive statuses that are not `OP_OK`.

## 5. Behavioral Design

### 5.1 Control path

On `controlIn`:

- `START`: set `m_enabled = true`, emit `AcquisitionEnabled`.
- `STOP`: set `m_enabled = false`, emit `AcquisitionDisabled`.
- `RESET_PARSER`: clear parser buffer and all counters, emit `ParserReset`.

### 5.2 Receive path

On `lidarDataIn`:

1. If status is not `OP_OK`:
   - Increment `DriverDropCount`.
   - Emit `DriverReceiveDropped`.
   - Return input buffer to `lidarDataReturnOut`.
2. If status is `OP_OK` and acquisition is enabled:
   - Iterate each byte and feed parser state machine.
   - For each complete valid frame, decode payload and emit data.
3. Always return input buffer to `lidarDataReturnOut` when connected.

### 5.3 Parser logic

The parser uses a two-byte header synchronization and fixed frame length:

- Header bytes: `0x59 0x59`
- Frame length: 9 bytes total
- Checksum: sum of bytes `[0..7]`, compared against byte `[8]`

Decoded fields:

- `distanceCm = (byte3 << 8) | byte2`
- `signalStrength = (byte5 << 8) | byte4`
- `temperatureCentiC = (byte7 << 8) | byte6`

On valid frame:

- Update telemetry (`DistanceCm`, `SignalStrength`, `TemperatureCentiC`, `FrameCount`).
- Emit parsed frame on `frameOut` when connected.

On header/checksum failure:

- Increment associated counter telemetry.
- Emit warning event (`HeaderSyncError` or `ChecksumError`).

## 6. Telemetry and Events

### 6.1 Telemetry

- `DistanceCm`
- `SignalStrength`
- `TemperatureCentiC`
- `FrameCount`
- `ChecksumErrorCount`
- `HeaderErrorCount`
- `DriverDropCount`

### 6.2 Events

- Activity high: `DriverReady`, `AcquisitionEnabled`, `AcquisitionDisabled`, `ParserReset`
- Warning high: `ChecksumError`, `HeaderSyncError`, `DriverReceiveDropped`

## 7. Assumptions and Constraints

1. UART stream carries TF-Luna frame format with standard headers and checksum semantics.
2. Driver provides valid buffers through `Drv.ByteStreamData`.
3. Buffer ownership is returned by manager after processing every receive callback.
4. Topology wiring enforces one manager instance per TF-Luna stream.

## 8. Integration Notes

Recommended topology connections:

- `MarsAnalyzer.managerControlOut -> TfLunaManager.controlIn`
- `TfLunaManager.frameOut -> MarsAnalyzer.managerFrameIn`
- `LinuxUartDriver.ready -> TfLunaManager.lidarDriverReady`
- `LinuxUartDriver.$recv -> TfLunaManager.lidarDataIn`
- `TfLunaManager.lidarDataReturnOut -> LinuxUartDriver.recvReturnIn`

This preserves strict layering and keeps mission logic independent from driver protocol details.
