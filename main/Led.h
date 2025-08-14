/**
 * @file Led.h
 * @brief Simple GPIO output wrapper used for indicator LEDs.
 */

#pragma once
#include "driver/gpio.h"

/** @brief Minimal class providing on/off/toggle control of a GPIO pin. */
class Led {
public:
    /**
     * @brief Create a LED bound to the given GPIO pin.
     * @param pin GPIO pin connected to the LED.
     */
    explicit Led(gpio_num_t pin);

    /**
     * @brief Drive the pin HIGH when on is true, LOW otherwise.
     * @param on Desired output state.
     */
    void set(bool on);

    /**
     * @brief Invert the current state.
     */
    void toggle();

    /**
     * @brief Query the current state.
     * @return true if the pin is HIGH.
     */
    bool get() const;

private:
    gpio_num_t pin_; // GPIO connected to the LED
};
