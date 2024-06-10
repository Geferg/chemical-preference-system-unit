#ifndef TCPDEBUGCLEAN_HENUMS_H
#define TCPDEBUGCLEAN_ENUMS_H

namespace Formatting {
    enum InstructionType {
        Comment,
        SwitchIllumination,
        SwitchValve,
        RunMotor,
        StopMotor,
        Wait,
        Calibrate,
        Status
    };
}

#endif
