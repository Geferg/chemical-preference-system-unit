#ifndef TCPDEBUGCLEAN_HENUMS_H
#define TCPDEBUGCLEAN_HENUMS_H

namespace Hardware {
    enum ValveCluster {
        right,
        left,
        bottom
    };

    enum ValveMode {
        buffer,
        product,
        circuit,
        trash
    };

    enum MotorDirection {
        up,
        down
    };
}

#endif
