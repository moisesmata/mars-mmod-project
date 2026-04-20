module Mars {
    @ Passive manager component that handles TF-Luna UART protocol and driver integration
    passive component TfLunaManager {

        @ Port that reads data from device
        output port read: Drv.I2c

        @ Port that writes data to device
        output port write: Drv.I2c

        sync input port run: Svc.Sched

        telemetry distance: U32

        telemetry flux: U32

        telemetry temperature: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut


        
    }
}
