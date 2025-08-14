// Led.cpp - implementation of simple GPIO LED helper

#include "Led.h"

// Configure the GPIO pin for output and start in the OFF state
Led::Led(gpio_num_t pin) : pin_(pin) {
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = 1ULL << pin_;
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);
    set(false); // ensure LED is initially off
}

// Drive the LED to the requested state
void Led::set(bool on) {
    gpio_set_level(pin_, on ? 1 : 0);
}

// Invert the LED state
void Led::toggle() {
    set(!get());
}

// Query the current output level
bool Led::get() const {
    return gpio_get_level(pin_) != 0;
}
