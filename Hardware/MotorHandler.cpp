//
// Created by krist on 10.06.2024.
//

#include <pigpio.h>
#include <chrono>
#include <iostream>
#include "MotorHandler.h"
#include "HEnums.h"

namespace Hardware {
    volatile bool Hardware::MotorHandler::limit = false;
    volatile MotorHandler::LimitStatus MotorHandler::limitStatus = MotorHandler::LimitStatus::none;
    volatile MotorHandler::LimitStatus MotorHandler::sensedLimit = MotorHandler::LimitStatus::none;

    int MotorHandler::prepareGpio() {
        int ret = gpioSetMode(dir_pin, PI_OUTPUT);
        if (ret != 0) {
            return ret;
        }

        ret = gpioSetMode(step_pin, PI_OUTPUT);
        if (ret != 0) {
            return ret;
        }

        ret = gpioSetMode(sleep_pin, PI_OUTPUT);
        if (ret != 0) {
            return ret;
        }

        gpioSetISRFunc(top_limit_button_pin, FALLING_EDGE, 0, on_limit);
        gpioSetISRFunc(bottom_limit_button_pin, FALLING_EDGE, 0, on_limit);

        gpioWrite(sleep_pin, PI_LOW);

        return 0;
    }

    int MotorHandler::runFor(MotorDirection direction, unsigned int step_period_micros,
                             unsigned int run_period_micros) {
        if (step_period_micros < min_step_period) {
            return -1;
        }

        gpioWrite(sleep_pin, PI_HIGH);

        gpioDelay(2000);

        gpioWrite(step_pin, PI_LOW);
        if (direction == down) {
            gpioWrite(dir_pin, PI_LOW);
        }
        else {
            gpioWrite(dir_pin, PI_HIGH);
        }

        auto start = std::chrono::high_resolution_clock::now();

        long cycle_time;
        long cycle_error = 0;
        int real_steps = 0;

        int stopCount = 0;

        for(unsigned int i = 0; i < run_period_micros; i += step_period_micros) {

            //TODO Break on limit
            /*
            if (direction == MotorDirection::down && sensedLimit == LimitStatus::limitDown) {
                stopCount++;
                if (stopCount > 3) {
                    std::cout << "Stopping motor, bottom limit reached\n";
                    break;  // Break the loop if the bottom limit is reached while moving down
                }
            }
            else if (direction == MotorDirection::up && sensedLimit == LimitStatus::limitUp) {
                stopCount++;
                if (stopCount > 3) {
                    std::cout << "Stopping motor, bottom limit reached\n";
                    break;  // Break the loop if the bottom limit is reached while moving down
                }
            }
             */

            long compensate_micros = getCompensationMicros(cycle_error);

            auto cycle_start = std::chrono::high_resolution_clock::now();

            if (step_period_micros - compensate_micros < 0) {
                compensate_micros = step_period_micros - 4;
            }

            gpioWrite(step_pin, PI_HIGH);
            gpioDelay((step_period_micros - compensate_micros)/2);
            gpioWrite(step_pin, PI_LOW);
            gpioDelay((step_period_micros - compensate_micros)/2);

            auto cycle_stop = std::chrono::high_resolution_clock::now();

            cycle_time = std::chrono::duration_cast<std::chrono::microseconds>(cycle_stop - cycle_start).count();
            cycle_error = cycle_time - step_period_micros;

            real_steps++;
        }

        // UNCOMMENT FOR TUNING COMPENSATOR PID
        //auto stop = std::chrono::high_resolution_clock::now();
        //auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        //long error = runtime.count() - run_period_millis * 1000;
        //float error_ratio = (float)error/((float)run_period_millis * 1000);
        //float error_per_cycle = (float)error/(float)cycle_count;
        //std::cout << "Absolute error: " << error << "μs\n";
        //std::cout << "Percentage error: " << std::floor(error_ratio * 10000) / 100 <<"%\n";
        //std::cout << "Cycles: " << cycle_count << "\n";
        //std::cout << "Error per cycle: " << error_per_cycle << "μs\n";
        std::cout << "Real step count: " << real_steps << "\n";

        gpioWrite(dir_pin, PI_LOW);
        gpioWrite(sleep_pin, PI_LOW);
        clearCompensation();

        return 0;
    }

    long MotorHandler::getCompensationMicros(long error_micros) {
        // PI-Controller
        // replacing `error_micros` with `(total_error/cycle_count)` could smooth result in some cases
        total_error += error_micros;
        cycle_count++;

        long comp = error_micros/5 + total_error/10;

        return comp;
    }

    void MotorHandler::clearCompensation() {
        last_error = 0;
        total_error = 0;
        cycle_count = 0;
    }

    void MotorHandler::on_limit(int pin, int level, uint32_t tick) {
        //TODO Handle limit position gracefully
        if(pin == bottom_limit_button_pin) {
            std::cout << "bottom triggered\n";
            limit = true;
            sensedLimit = LimitStatus::limitDown;
            //while(true);
        }
        else if(pin == top_limit_button_pin) {
            std::cout << "top triggered\n";
            limit = true;
            sensedLimit = LimitStatus::limitUp;
            //while(true);
        }


    }
}