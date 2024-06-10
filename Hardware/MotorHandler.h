#ifndef TCPDEBUGCLEAN_MOTORHANDLER_H
#define TCPDEBUGCLEAN_MOTORHANDLER_H

#include "HEnums.h"

namespace Hardware {

    class MotorHandler {
    public:
        bool compensate;

        explicit MotorHandler() {
            compensate = true;
            total_error = 0;
            last_error = 0;
            cycle_count = 0;
        }

        int prepareGpio();
        int runFor(MotorDirection direction, unsigned int step_period_micros, unsigned int run_period_micros);

        enum LimitStatus {
            none,
            limitUp,
            limitDown
        };
    private:

        static volatile LimitStatus limitStatus;
        static volatile LimitStatus sensedLimit;

        static volatile bool limit;

        static const int min_step_period = 500;

        static const unsigned int top_limit_button_pin = 11;
        static const unsigned int bottom_limit_button_pin = 9;
        static const unsigned int dir_pin = 20;
        static const unsigned int step_pin = 12;
        static const unsigned int sleep_pin = 16;

        long total_error;
        long last_error;
        int cycle_count;

        long getCompensationMicros(long error_micros);
        void clearCompensation();

        static void on_limit(int pin, int level, uint32_t tick);
    };
}

#endif
