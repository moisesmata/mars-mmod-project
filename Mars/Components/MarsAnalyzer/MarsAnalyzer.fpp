module Mars {
    @ Active application component implementing mission-specific MMOD analysis logic
    passive component MarsAnalyzer {

        @ Measurement input from the TF-Luna manager
        sync input port managerFrameIn: Mars.TfLunaFrame

        # ----------------------------------------------------------------------
        # MMOD detection
        # ----------------------------------------------------------------------

        @ Distance threshold (cm). A reading at or below this value is treated as an MMOD event.
        param MmodThresholdCm: U16 default 30

        @ Most recent distance reading observed from the TF-Luna manager (cm).
        telemetry LastDistanceCm: U16

        @ Number of MMOD detections since boot.
        telemetry MmodCount: U32

        @ Emitted when a distance reading crosses below the configured MMOD threshold.
        event MmodDetected($distance: U16) \
            severity warning high \
            format "MMOD detected: distance {} cm"

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}