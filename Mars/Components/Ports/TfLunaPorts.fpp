module Mars {
    @ Control actions sent from application layer to TF-Luna manager
    enum TfLunaControlAction : U8 {
        START
        STOP
        RESET_PARSER
    }

    @ Application to manager control interface
    port TfLunaControl(controlAction: TfLunaControlAction)

    @ Manager to application decoded frame interface
    port TfLunaFrame(distanceCm: U16, signalStrength: U16, temperatureCentiC: I16)
}
