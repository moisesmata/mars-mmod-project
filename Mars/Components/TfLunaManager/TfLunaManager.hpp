// ======================================================================
// \title  TfLunaManager.hpp
// \author moisesm
// \brief  hpp file for TfLunaManager component implementation class
// ======================================================================

#ifndef Mars_TfLunaManager_HPP
#define Mars_TfLunaManager_HPP

// Register Definitions
#define DEFAULT_ADDRESS 0x10  // default I2C address = 16
#define TFL_DEF_FPS 0x64  // default frame-rate = 100fps

#define DIST_LOW 0x00  // Unit: cm
#define DIST_HIGH 0x01
#define AMP_LOW 0x02
#define FLUX_HIGH 0x03
#define TEMP_LOW 0x04  // Unit: 0.01 Celsius
#define TEMP_HIGH 0x05
#define TICK_LOW 0x06  // Timestamp
#define TICK_HIGH 0x07
#define ERROR_LOW 0x08
#define ERROR_HIGH 0x09
#define VERSION_REVISION 0x0A
#define VERSION_MINOR 0x0B
#define VERSION_MAJOR 0x0C

// HOLD 0x0D-0x0F

#define SAVE 0x20             // Write 0x01 to save current setting
#define SHUTDOWN_REBOOT 0x21  // Write 0x02 to reboot
#define SLAVE_ADDR 0x22     // (default: 0x10) Set the I2C Slave Address (Must reboot to take effect) Range: 0x08 - 0x77
#define MODE 0x23           // 0x00 (default): Continuous Ranging mode, 0x01: Trigger Mode
#define TRIG_ONE_SHOT 0x24  // 0x01: Trigger once
#define ENABLE 0x25         // (default: 0x01) 0x00: Turn off LiDAR
#define FPS_LOW 0x26        // (default: 0x64)
#define FPS_HIGH 0x27       // (default: 0x00)
#define LOW_POWER 0x28      // (default: 0x00) 0x00: Normal, 0x01: Power Saving Mode
#define RESTORE_FACTORY_DEFAULTS  // Write 0x01 to restore factory default settings (Hard Reset)

// Error Status Condition (from ref)
#define TFL_READY 0     // no error
#define TFL_SERIAL 1    // serial timeout
#define TFL_HEADER 2    // no header found
#define TFL_CHECKSUM 3  // checksum doesn't match
#define TFL_TIMEOUT 4   // I2C timeout
#define TFL_PASS 5      // reply from some system commands
#define TFL_FAIL 6      //           "
#define TFL_I2CREAD 7
#define TFL_I2CWRITE 8
#define TFL_I2CLENGTH 9
#define TFL_WEAK 10    // Signal Strength ≤ 100
#define TFL_STRONG 11  // Signal Strength saturation
#define TFL_FLOOD 12   // Ambient Light saturation
#define TFL_MEASURE 13
#define TFL_INVALID 14  // Invalid operation sent to sendCommand()

#include <array>

#include "Mars/Components/TfLunaManager/TfLunaManagerComponentAc.hpp"

namespace Mars {

class TfLunaManager final : public TfLunaManagerComponentBase {
  public:
    // Constructor + Destructor
    TfLunaManager(const char* const compName);

    ~TfLunaManager();

    Drv::I2cStatus setupReadRegister(U8 reg);
    Drv::I2cStatus readRegisterBlock(U8 startRegister, Fw::Buffer &buffer);

    bool getData();

   
  private:
    U8 MAX_DATA_SIZE_BYTES = 8;
    U8 distance;
    U8 temp;
    U8 flux;

};

}  // namespace Mars

#endif
