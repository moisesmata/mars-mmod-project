####
# F Prime CMakeLists.txt for TfLunaManager component
####

register_fprime_library(
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/TfLunaManager.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/TfLunaManager.cpp"
    DEPENDS
        Drv_ByteStreamDriverModel
        Mars_Components_Ports
)
