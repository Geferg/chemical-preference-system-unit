#ifndef TCPDEBUGCLEAN_VALVEHANDLER_H
#define TCPDEBUGCLEAN_VALVEHANDLER_H

#include "HEnums.h"

namespace Hardware {

    class ValveHandler {
    public:
        explicit ValveHandler(const unsigned int valve_pins[6]) {
            for(int i = 0; i < 6; i++) {
                valve_gpio_pins[i] = valve_pins[i];
            }
        }

        //int prepareGpio();

        int initialize();
        /*
         *  |         |   Left    |   Right   |  Bottom   |
         *  |---------|-----|-----|-----|-----|-----|-----|
         *  |  Mode   |  0  |  1  |  2  |  3  |  4  |  5  |
         *  |---------|-----|-----|-----|-----|-----|-----|
         *  | Buffer  | 0   | 0   | 0   | 0   | n/a | n/a |
         *  | Product | 0   | 1   | 1   | 0   | n/a | n/a |
         *  | Circuit | 1   | x   | x   | 1   | 0   | 0   |
         *  | Trash   | n/a | n/a | n/a | n/a | 1   | 1   |
         */
        int set(ValveCluster valves, ValveMode mode);
        int zero();
        int force();
    private:
        unsigned int valve_gpio_pins[6]{};
    };

} // Hardware

#endif //TCPDEBUGCLEAN_VALVEHANDLER_H
