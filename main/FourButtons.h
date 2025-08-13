#pragma once
#include <stdint.h>
#include "driver/gpio.h"

// One struct per button: which pin and which ID to print.
struct ButtonSpec {
    gpio_num_t pin;
    uint8_t    id;
};

// Debounced 4-button reader. Active-LOW by default (internal pull-ups on).
class FourButtons {
public:
    // specs: 4 entries. debounceMs typical 30. activeLow=true for pull-ups.
    FourButtons(const ButtonSpec specs[4], int debounceMs, bool activeLow = true);

    // Call this every dtMs (e.g., dtMs = 5 in a FreeRTOS loop).
    void poll(int dtMs);

private:
    ButtonSpec btn_[4];
    int  debounceMs_;
    bool activeLow_;

    // state per button
    bool lastRaw_[4] = { false,false,false,false };
    bool stableDown_[4] = { false,false,false,false };
    int  timerMs_[4] = { 0,0,0,0 };
};
