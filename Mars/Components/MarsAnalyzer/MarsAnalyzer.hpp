// ======================================================================
// \title  MarsAnalyzer.hpp
// \author moisesm
// \brief  hpp file for MarsAnalyzer component implementation class
// ======================================================================

#ifndef Mars_MarsAnalyzer_HPP
#define Mars_MarsAnalyzer_HPP

#include "Mars/Components/MarsAnalyzer/MarsAnalyzerComponentAc.hpp"

namespace Mars {

class MarsAnalyzer final : public MarsAnalyzerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct MarsAnalyzer object
    MarsAnalyzer(const char* const compName  //!< The component name
    );

    //! Destroy MarsAnalyzer object
    ~MarsAnalyzer();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command START
    void START_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) override;

    //! Handler implementation for command STOP
    void STOP_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) override;

    //! Handler implementation for command RESET_PARSER
    void RESET_PARSER_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) override;

    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for managerFrameIn
    void managerFrameIn_handler(FwIndexType portNum,
                                U16 distanceCm,
                                U16 signalStrength,
                                I16 temperatureCentiC) override;
};

}  // namespace Mars

#endif
