// Led.h - simple GPIO output wrapper used for indicator LEDs

#pragma once
#include "driver/gpio.h"

// Minimal class providing on/off/toggle control of a GPIO pin
class Led {
public:
    // Create a LED bound to the given GPIO pin
    explicit Led(gpio_num_t pin);

    // Drive the pin HIGH when "on" is true, LOW otherwise
    void set(bool on);

    // Convenience method to invert the current state
    void toggle();

    // Query the current state (true if HIGH)
    bool get() const;

private:
    gpio_num_t pin_; // GPIO connected to the LED
};
