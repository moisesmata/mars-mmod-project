// ======================================================================
// \title  TfLunaManager.cpp
// \author moisesm, robertpendergrast
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

// Configure the TfLuna to have a certain frequency
void TfLunaManager::configure_fr(U16 frequency){ 
    U8 low = frequency & 0xFF; // and with 256, giving us the lower 8 bits
    Fw::Buffer low_buffer(&low, sizeof low);
    this->write_out(0,FPS_LOW, low_buffer);

    U8 high = frequency >> 8; // shift right by 8, getting the upper 8 bits
    Fw::Buffer high_buffer(&low, sizeof low);
    this->write_out(0,FPS_HIGH, high_buffer);
};

bool TfLunaManager::getData(){
    U8 data[MAX_DATA_SIZE_BYTES]; // Max data size = 8
    Fw::Buffer buffer(data, sizeof(data));

    Drv::I2cStatus status = this->readRegisterBlock(DIST_LOW, buffer);

    // Check the status of the read first
    if((status == Drv::I2cStatus::I2C_OK) && (buffer.getSize() == 6) && buffer.getData() != nullptr){
        U8* raw_data = buffer.getData();

        // Shift high byte over and add
        this->distance = static_cast<U16>(raw_data[0] | (raw_data[1] << 8));
        this->flux     = static_cast<U16>(raw_data[2] | (raw_data[3] << 8));
        this->temp     = static_cast<I16>(raw_data[4] | (raw_data[5] << 8));

        return true;
    }

    // Return false on bad read
    return false;
};

void TfLunaManager::run_handler(FwIndexType portNum, U32 context){
    if (!this->enabled) {
        return;
    }

    if (!this->getData()) {
        return;
    }

    this->tlmWrite_distance(this->distance);
    this->tlmWrite_flux(this->flux);
    this->tlmWrite_temperature(this->temp);

    // Forward the decoded frame to another component if desired
    if (this->isConnected_frameOut_OutputPort(0)) {
        this->frameOut_out(0, this->distance, this->flux, this->temp);
    }
};

void TfLunaManager::CONTROL_cmdHandler(FwOpcodeType opCode,
                                       U32 cmdSeq,
                                       Mars::TfLunaControlAction action) {
    switch (action.e) {
        case Mars::TfLunaControlAction::START:
            this->enabled = true;
            break;
        case Mars::TfLunaControlAction::STOP:
            this->enabled = false;
            break;
        default:
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::VALIDATION_ERROR);
            return;
    }

    this->log_ACTIVITY_LO_ControlActionApplied(action);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}



}  // namespace Mars
