// ======================================================================
// \title  TfLunaManager.hpp
// \author moisesm
// \brief  hpp file for TfLunaManager component implementation class
// ======================================================================

#ifndef Mars_TfLunaManager_HPP
#define Mars_TfLunaManager_HPP

#include <array>

#include "Mars/Components/TfLunaManager/TfLunaManagerComponentAc.hpp"

namespace Mars {

class TfLunaManager final : public TfLunaManagerComponentBase {
  public:
    //! Construct TfLunaManager object
    TfLunaManager(const char* const compName);

    //! Destroy TfLunaManager object
    ~TfLunaManager();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for controlIn
    void controlIn_handler(FwIndexType portNum, const TfLunaControlAction& action) override;

    //! Handler implementation for lidarDriverReady
    void lidarDriverReady_handler(FwIndexType portNum) override;

    //! Handler implementation for lidarDataIn
    void lidarDataIn_handler(FwIndexType portNum, Fw::Buffer& buffer, const Drv::ByteStreamStatus& status) override;

    // ----------------------------------------------------------------------
    // Internal helpers
    // ----------------------------------------------------------------------

    //! Reset parser state and statistics
    void resetParserState();

    //! Process one incoming byte, updating parser state
    void processByte(U8 byte);

    //! Decode and publish a complete frame currently in m_frame
    void decodeFrame();

  private:
    static constexpr U8 TF_LUNA_HEADER = 0x59;
    static constexpr U8 TF_LUNA_FRAME_LENGTH = 9;

    bool m_enabled;
    std::array<U8, TF_LUNA_FRAME_LENGTH> m_frame;
    U8 m_frameFill;

    U32 m_frameCount;
    U32 m_checksumErrorCount;
    U32 m_headerErrorCount;
    U32 m_driverDropCount;
};

}  // namespace Mars

#endif
