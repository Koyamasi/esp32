#pragma once
#include "driver/gpio.h"

class Led {
public:
    explicit Led(gpio_num_t pin);
    void set(bool on);
    void toggle();
    bool get() const;
private:
    gpio_num_t pin_;
};
