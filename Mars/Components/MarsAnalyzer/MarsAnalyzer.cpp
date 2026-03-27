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

MarsAnalyzer::MarsAnalyzer(const char* const compName) : MarsAnalyzerComponentBase(compName) {}

MarsAnalyzer::~MarsAnalyzer() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void MarsAnalyzer::START_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    if (this->isConnected_managerControlOut_OutputPort(0)) {
        this->managerControlOut_out(0, TfLunaControlAction::START);
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void MarsAnalyzer::STOP_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    if (this->isConnected_managerControlOut_OutputPort(0)) {
        this->managerControlOut_out(0, TfLunaControlAction::STOP);
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void MarsAnalyzer::RESET_PARSER_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    if (this->isConnected_managerControlOut_OutputPort(0)) {
        this->managerControlOut_out(0, TfLunaControlAction::RESET_PARSER);
    }
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void MarsAnalyzer::managerFrameIn_handler(FwIndexType portNum,
                                          U16 distanceCm,
                                          U16 signalStrength,
                                          I16 temperatureCentiC) {
    static_cast<void>(portNum);
    // Mission-specific application logic belongs here.
    static_cast<void>(distanceCm);
    static_cast<void>(signalStrength);
    static_cast<void>(temperatureCentiC);
}

}  // namespace Mars
