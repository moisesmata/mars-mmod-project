// ======================================================================
// \title  MarsAnalyzer.cpp
// \author moisesm
// \brief  cpp file for MarsAnalyzer component implementation class
// ======================================================================

#include "Mars/Components/MarsAnalyzer/MarsAnalyzer.hpp"

namespace Mars {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

MarsAnalyzer::MarsAnalyzer(const char* const compName)
    : MarsAnalyzerComponentBase(compName),
      m_appFrameCount(0) {}

MarsAnalyzer::~MarsAnalyzer() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void MarsAnalyzer::START_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    if (this->isConnected_managerControlOut_OutputPort(0)) {
        this->managerControlOut_out(0, TfLunaControlAction::START);
    }
    this->log_ACTIVITY_HI_AcquisitionStarted();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void MarsAnalyzer::STOP_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    if (this->isConnected_managerControlOut_OutputPort(0)) {
        this->managerControlOut_out(0, TfLunaControlAction::STOP);
    }
    this->log_ACTIVITY_HI_AcquisitionStopped();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void MarsAnalyzer::RESET_PARSER_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    if (this->isConnected_managerControlOut_OutputPort(0)) {
        this->managerControlOut_out(0, TfLunaControlAction::RESET_PARSER);
    }
    this->log_ACTIVITY_HI_ParserResetRequested();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void MarsAnalyzer::managerFrameIn_handler(FwIndexType portNum,
                                          U16 distanceCm,
                                          U16 signalStrength,
                                          I16 temperatureCentiC) {
    static_cast<void>(portNum);
    this->m_appFrameCount++;
    this->tlmWrite_AppDistanceCm(distanceCm);
    this->tlmWrite_AppSignalStrength(signalStrength);
    this->tlmWrite_AppTemperatureCentiC(temperatureCentiC);
    this->tlmWrite_AppFrameCount(this->m_appFrameCount);
}

}  // namespace Mars
