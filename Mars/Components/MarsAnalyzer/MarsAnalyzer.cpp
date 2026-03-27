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

MarsAnalyzer ::MarsAnalyzer(const char* const compName) : MarsAnalyzerComponentBase(compName) {}

MarsAnalyzer ::~MarsAnalyzer() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void MarsAnalyzer ::TODO_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace Mars
