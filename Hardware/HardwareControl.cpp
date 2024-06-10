#include <pigpio.h>
#include <iostream>
#include "HardwareControl.h"

namespace Hardware {
    ValveHandler HardwareControl::valveHandler = ValveHandler(valve_gpio_pins);
    MotorHandler HardwareControl::motorHandler = MotorHandler();

    bool HardwareControl::gpio_ready = false;

    int HardwareControl::initialize() {
        if (gpioInitialise() == -1) {
            return -1;
        }

        if (valveHandler.initialize() != 0) {
            return -1;
        }

        if (motorHandler.prepareGpio() != 0) {
            return -1;
        }

        motorHandler.compensate = false;
        gpio_ready = true;

        return 0;
    }

    int HardwareControl::setValveMode(ValveCluster valves, ValveMode mode) {
        if (!gpio_ready) {
            return -1;
        }

        return valveHandler.set(valves, mode);
    }

    int HardwareControl::resetValves() {
        if(!gpio_ready) {
            return -1;
        }

        return valveHandler.zero();
    }

    int HardwareControl::runMotorByTime(MotorDirection direction,
                                        unsigned int time_us,
                                        unsigned int period_us_per_step) {
        if (!gpio_ready) {
            return -2;
        }

        std::cout << "period: " << period_us_per_step << "us\n";
        std::cout << "time: " << time_us << "us\n";

        return motorHandler.runFor(direction, period_us_per_step, time_us);
    }

    int HardwareControl::runMotorByDistance(MotorDirection direction,
                                            float distance_mm,
                                            float velocity_mm_per_s) {
        unsigned int period_us_per_step = (int)(2500/velocity_mm_per_s);
        unsigned int time_us = (int)((float)period_us_per_step * 400 * distance_mm);

        return runMotorByTime(direction, time_us, period_us_per_step);
    }

    int HardwareControl::enableLed() {
        if (!gpio_ready) {
            return -1;
        }

        return gpioWrite(ir_light_pin, PI_LOW);
    }

    int HardwareControl::disableLed() {
        if (!gpio_ready) {
            return -1;
        }

        return gpioWrite(ir_light_pin, PI_HIGH);
    }

    bool HardwareControl::isReady() {
        return gpio_ready;
    }

    int HardwareControl::delay(int millis) {
        return (int)gpioDelay(millis * 1000);
    }
}