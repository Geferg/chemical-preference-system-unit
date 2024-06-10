#ifndef TCPDEBUGCLEAN_HARDWARECONTROL_H
#define TCPDEBUGCLEAN_HARDWARECONTROL_H

#include "HEnums.h"
#include "ValveHandler.h"
#include "MotorHandler.h"

namespace Hardware {
    class HardwareControl {
    public:
        static int initialize();

        // |  Mode   | V1  | V2  | V3  | V4  | V5  | V6  |
        // |---------|-----|-----|-----|-----|-----|-----|
        // | Buffer  |  0  |  0  |  0  |  0  | n/a | n/a |
        // | Product |  0  |  1  |  1  |  0  | n/a | n/a |
        // | Circuit |  1  |  x  |  x  |  1  |  0  |  0  |
        // | Trash   | n/a | n/a | n/a | n/a |  1  |  1  |
        static int setValveMode(ValveCluster valves, ValveMode mode);
        static int resetValves();

        static int runMotorByTime(MotorDirection direction, unsigned int time_us, unsigned int period_us_per_step);
        static int runMotorByDistance(MotorDirection direction, float distance_mm, float velocity_mm_per_s);

        static int enableLed();
        static int disableLed();

        static bool isReady();
        static int delay(int millis);

    private:
        static ValveHandler valveHandler;
        static MotorHandler motorHandler;

        //static const unsigned int visible_light_pin = 17;
        static const unsigned int ir_light_pin = 24;

        static const unsigned int top_limit_button = 11;
        static const unsigned int bottom_limit_button = 9;
        static const unsigned int motor_dir_pin = 20;
        static const unsigned int motor_step_pin = 12;
        static const unsigned int motor_sleep_pin = 16;

        static const unsigned int valve_1_gpio_pin = 7;
        static const unsigned int valve_2_gpio_pin = 1;
        static const unsigned int valve_3_gpio_pin = 8;
        static const unsigned int valve_4_gpio_pin = 25;
        static const unsigned int valve_5_gpio_pin = 22;
        static const unsigned int valve_6_gpio_pin = 23;


        constexpr static const unsigned int valve_gpio_pins[6] = {
                valve_1_gpio_pin,
                valve_2_gpio_pin,
                valve_3_gpio_pin,
                valve_4_gpio_pin,
                valve_5_gpio_pin,
                valve_6_gpio_pin
        };

        static bool gpio_ready;
    };
}

#endif
