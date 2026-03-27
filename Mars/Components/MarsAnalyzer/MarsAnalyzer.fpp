module Mars {
    @ Active application component implementing mission-specific MMOD analysis logic
    active component MarsAnalyzer {

        @ Start TF-Luna acquisition in the manager
        async command START opcode 0

        @ Stop TF-Luna acquisition in the manager
        async command STOP opcode 1

        @ Reset manager parser state and statistics
        async command RESET_PARSER opcode 2

        @ Control output to the TF-Luna manager
        output port managerControlOut: Mars.TfLunaControl

        @ Measurement input from the TF-Luna manager
        sync input port managerFrameIn: Mars.TfLunaFrame

        @ Latest distance from manager, in centimeters
        telemetry AppDistanceCm: U16

        @ Latest signal strength from manager
        telemetry AppSignalStrength: U16

        @ Latest temperature from manager, in centi-degrees Celsius
        telemetry AppTemperatureCentiC: I16

        @ Number of frames received from manager
        telemetry AppFrameCount: U32

        @ Acquisition started by command
        event AcquisitionStarted severity activity high id 0 \
          format "Application requested TF-Luna start"

        @ Acquisition stopped by command
        event AcquisitionStopped severity activity high id 1 \
          format "Application requested TF-Luna stop"

        @ Parser reset requested by command
        event ParserResetRequested severity activity high id 2 \
          format "Application requested TF-Luna parser reset"

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

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