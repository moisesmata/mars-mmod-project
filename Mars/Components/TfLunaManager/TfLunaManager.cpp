// ======================================================================
// \title  TfLunaManager.cpp
// \author moisesm
// \brief  cpp file for TfLunaManager component implementation class
// ======================================================================

#include "Mars/Components/TfLunaManager/TfLunaManager.hpp"
#include "Fw/Types/Assert.hpp"

namespace Mars {

TfLunaManager::TfLunaManager(const char* const compName)
    : TfLunaManagerComponentBase(compName) {}

TfLunaManager::~TfLunaManager() {}

// Member function definitions

// Tell the device what register we want to read from
Drv::I2cStatus TfLunaManager::setupReadRegister(U8 reg){
    Fw::Buffer buffer(&reg, sizeof reg);
    return this->write_out(0, DEFAULT_ADDRESS, buffer); // default = 0x10
}

// Read a chunk of registers from the device
Drv::I2cStatus TfLunaManager::readRegisterBlock(U8 startRegister, Fw::Buffer &buffer){
    Drv::I2cStatus status;
    status = this->setupReadRegister(startRegister);
    if (status == Drv::I2cStatus::I2C_OK) {
        status = this->read_out(0, DEFAULT_ADDRESS, buffer); // default = 0x10
    }
    return status;
}

bool TfLunaManager::getData(){
    U8 data[MAX_DATA_SIZE_BYTES];
    Fw::Buffer buffer(data, sizeof(data));

    Drv::I2cStatus status = this->readRegisterBlock(DIST_LOW, buffer);

    // Check the status of the read first
    if((status == Drv::I2cStatus::I2C_OK) && (buffer.getSize() == 6) && buffer.getData() != nullptr){
        // Do the necessary bit shifting for the data
        U8* raw = buffer.getData();

        this->distance = raw[0] + (raw[1] << 8);
        this->flux = raw[2] + (raw[3] << 8);
        this->temp = raw[4] + (raw[5] << 8);

        return true;
    }

    // Return false on bad read
    return false;
};



}  // namespace Mars
