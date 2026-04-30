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
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void MarsAnalyzer::managerFrameIn_handler(FwIndexType portNum,
                                          U16 distanceCm,
                                          U16 signalStrength,
                                          I16 temperatureCentiC) {

    this->tlmWrite_LastDistanceCm(distanceCm);

    Fw::ParamValid valid = Fw::ParamValid::INVALID;
    const U16 threshold = this->paramGet_MmodThresholdCm(valid);

    if (distanceCm > 0 && distanceCm <= threshold) {
        this->mmodCount++;
        this->tlmWrite_MmodCount(this->mmodCount);
        this->log_WARNING_HI_MmodDetected(distanceCm);
    }
}

}  // namespace Mars
