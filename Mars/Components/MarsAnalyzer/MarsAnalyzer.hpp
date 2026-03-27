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

    //! Handler implementation for command TODO
    //!
    //! TODO
    void TODO_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                         U32 cmdSeq            //!< The command sequence number
                         ) override;
};

}  // namespace Mars

#endif
