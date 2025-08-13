#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "EventQueue.h"
class Led;

class Button {
public:
    Button(gpio_num_t pin, uint8_t id, EventQueue& bus, int debounceMs, Led* mirror = nullptr);
    void poll(int dtMs);
private:
    gpio_num_t  pin_;
    uint8_t     id_;
    EventQueue& bus_;
    int         debounceMs_;
    Led*        mirror_;

    bool lastRaw_{false};
    bool stableDown_{false};
    int  timerMs_{0};
};
