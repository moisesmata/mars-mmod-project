module Mars {
    @ Passive manager component that handles TF-Luna UART protocol and driver integration
    passive component TfLunaManager {
        @ Control input from application layer
        sync input port controlIn: Mars.TfLunaControl

        @ Decoded frame output to application layer
        output port frameOut: Mars.TfLunaFrame

        @ Ready signal from byte-stream driver
        sync input port lidarDriverReady: Drv.ByteStreamReady

        @ Incoming byte-stream buffer from driver
        sync input port lidarDataIn: Drv.ByteStreamData

        @ Return ownership of received buffers to driver chain
        output port lidarDataReturnOut: Fw.BufferSend

        @ Latest decoded TF-Luna distance, in centimeters
        telemetry DistanceCm: U16

        @ Latest decoded TF-Luna signal strength
        telemetry SignalStrength: U16

        @ Latest decoded TF-Luna temperature, in centi-degrees Celsius
        telemetry TemperatureCentiC: I16

        @ Number of valid TF-Luna frames decoded
        telemetry FrameCount: U32

        @ Number of checksum mismatches observed
        telemetry ChecksumErrorCount: U32

        @ Number of header-sync errors observed
        telemetry HeaderErrorCount: U32

        @ Number of dropped frames due to driver status errors
        telemetry DriverDropCount: U32

        @ Driver connected and signaled ready
        event DriverReady severity activity high id 0 \
          format "LiDAR byte-stream driver ready"

        @ Acquisition has been enabled
        event AcquisitionEnabled severity activity high id 1 \
          format "TF-Luna manager acquisition enabled"

        @ Acquisition has been disabled
        event AcquisitionDisabled severity activity high id 2 \
          format "TF-Luna manager acquisition disabled"

        @ Parser reset completed
        event ParserReset severity activity high id 3 \
          format "TF-Luna manager parser reset"

        @ Frame rejected due to checksum mismatch
        event ChecksumError severity warning high id 4 \
          format "TF-Luna checksum mismatch"

        @ Frame rejected due to invalid header bytes
        event HeaderSyncError severity warning high id 5 \
          format "TF-Luna header sync error"

        @ Input buffer dropped due to non-OK driver status
        event DriverReceiveDropped(status: Drv.ByteStreamStatus) severity warning high id 6 \
          format "LiDAR receive dropped with status {}"

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel
    }
}
