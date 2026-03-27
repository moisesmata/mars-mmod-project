// ======================================================================
// \title  TfLunaManager.cpp
// \author moisesm
// \brief  cpp file for TfLunaManager component implementation class
// ======================================================================

#include "Mars/Components/TfLunaManager/TfLunaManager.hpp"
#include "Fw/Types/Assert.hpp"

namespace Mars {

TfLunaManager::TfLunaManager(const char* const compName)
    : TfLunaManagerComponentBase(compName),
      m_enabled(false),
      m_frame{},
      m_frameFill(0),
      m_frameCount(0),
      m_checksumErrorCount(0),
      m_headerErrorCount(0),
      m_driverDropCount(0) {}

TfLunaManager::~TfLunaManager() {}

void TfLunaManager::controlIn_handler(FwIndexType portNum, const TfLunaControlAction& action) {
    static_cast<void>(portNum);

    switch (action.e) {
        case TfLunaControlAction::START:
            this->m_enabled = true;
            this->log_ACTIVITY_HI_AcquisitionEnabled();
            break;
        case TfLunaControlAction::STOP:
            this->m_enabled = false;
            this->log_ACTIVITY_HI_AcquisitionDisabled();
            break;
        case TfLunaControlAction::RESET_PARSER:
            this->resetParserState();
            this->log_ACTIVITY_HI_ParserReset();
            break;
        default:
            FW_ASSERT(0, static_cast<FwAssertArgType>(action.e));
            break;
    }
}

void TfLunaManager::lidarDriverReady_handler(FwIndexType portNum) {
    static_cast<void>(portNum);
    this->log_ACTIVITY_HI_DriverReady();
}

void TfLunaManager::lidarDataIn_handler(FwIndexType portNum, Fw::Buffer& buffer, const Drv::ByteStreamStatus& status) {
    static_cast<void>(portNum);

    if (status != Drv::ByteStreamStatus::OP_OK) {
        this->m_driverDropCount++;
        this->tlmWrite_DriverDropCount(this->m_driverDropCount);
        this->log_WARNING_HI_DriverReceiveDropped(status);
    } else if (this->m_enabled) {
        const U8* bytes = reinterpret_cast<const U8*>(buffer.getData());
        const FwSizeType size = buffer.getSize();
        for (FwSizeType i = 0; i < size; i++) {
            this->processByte(bytes[i]);
        }
    }

    // Always return ownership of the incoming receive buffer to the driver chain.
    if (this->isConnected_lidarDataReturnOut_OutputPort(0)) {
        this->lidarDataReturnOut_out(0, buffer);
    }
}

void TfLunaManager::resetParserState() {
    this->m_frame.fill(0);
    this->m_frameFill = 0;
    this->m_frameCount = 0;
    this->m_checksumErrorCount = 0;
    this->m_headerErrorCount = 0;
    this->m_driverDropCount = 0;
}

void TfLunaManager::processByte(U8 byte) {
    if (this->m_frameFill == 0U) {
        if (byte == TF_LUNA_HEADER) {
            this->m_frame[0] = byte;
            this->m_frameFill = 1U;
        }
        return;
    }

    if (this->m_frameFill == 1U) {
        if (byte == TF_LUNA_HEADER) {
            this->m_frame[1] = byte;
            this->m_frameFill = 2U;
        } else {
            this->m_frameFill = 0U;
            this->m_headerErrorCount++;
            this->tlmWrite_HeaderErrorCount(this->m_headerErrorCount);
            this->log_WARNING_HI_HeaderSyncError();
        }
        return;
    }

    this->m_frame[this->m_frameFill] = byte;
    this->m_frameFill++;

    if (this->m_frameFill == TF_LUNA_FRAME_LENGTH) {
        this->decodeFrame();
        this->m_frameFill = 0U;
    }
}

void TfLunaManager::decodeFrame() {
    U8 checksum = 0U;
    for (U8 i = 0; i < (TF_LUNA_FRAME_LENGTH - 1U); i++) {
        checksum = static_cast<U8>(checksum + this->m_frame[i]);
    }

    if (checksum != this->m_frame[TF_LUNA_FRAME_LENGTH - 1U]) {
        this->m_checksumErrorCount++;
        this->tlmWrite_ChecksumErrorCount(this->m_checksumErrorCount);
        this->log_WARNING_HI_ChecksumError();
        return;
    }

    const U16 distanceCm = static_cast<U16>((this->m_frame[3] << 8) | this->m_frame[2]);
    const U16 signalStrength = static_cast<U16>((this->m_frame[5] << 8) | this->m_frame[4]);
    const I16 temperatureCentiC = static_cast<I16>((this->m_frame[7] << 8) | this->m_frame[6]);

    this->m_frameCount++;
    this->tlmWrite_DistanceCm(distanceCm);
    this->tlmWrite_SignalStrength(signalStrength);
    this->tlmWrite_TemperatureCentiC(temperatureCentiC);
    this->tlmWrite_FrameCount(this->m_frameCount);

    if (this->isConnected_frameOut_OutputPort(0)) {
        this->frameOut_out(0, distanceCm, signalStrength, temperatureCentiC);
    }
}

}  // namespace Mars
