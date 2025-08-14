// FourButtons.h - helper for reading four debounced buttons

#pragma once
#include <stdint.h>
#include "driver/gpio.h"

// One struct per button: which pin is used and which ID to report in events.
struct ButtonSpec {
    gpio_num_t pin; // GPIO pin the button is connected to
    uint8_t    id;  // Identifier printed when the button changes state
};

// Debounced 4-button reader. Active-LOW by default (internal pull-ups on).
// The class polls all four buttons and prints a line on the UART whenever
// a stable edge is detected.
class FourButtons {
public:
    // "specs" contains four ButtonSpec entries.  "debounceMs" controls the
    // debounce period.  If "activeLow" is true the pins are assumed to read
    // low when pressed (internal pull-ups enabled).
    FourButtons(const ButtonSpec specs[4], int debounceMs,
                bool activeLow = true);

    // Call this every dtMs (e.g., dtMs = 5 in a FreeRTOS loop).
    void poll(int dtMs);

private:
    ButtonSpec btn_[4]; // configuration of the four buttons
    int  debounceMs_;   // debounce period in milliseconds
    bool activeLow_;    // interpretation of logic level

    // per-button debounce state
    bool lastRaw_[4]   = { false,false,false,false }; // last sampled level
    bool stableDown_[4]= { false,false,false,false }; // debounced state
    int  timerMs_[4]   = { 0,0,0,0 };                 // timers for each button
};
