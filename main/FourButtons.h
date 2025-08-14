// FourButtons.h - helper for reading four debounced buttons

#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "EventQueue.h"

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
    // specs: 4 entries. debounceMs typical 30. activeLow=true for pull-ups.
    // Events for each edge are sent to bus with type ButtonEdge and id from specs.
    FourButtons(const ButtonSpec specs[4], EventQueue& bus, int debounceMs, bool activeLow = true);


    // Call this every dtMs (e.g., dtMs = 5 in a FreeRTOS loop).
    void poll(int dtMs);

private:
    ButtonSpec   btn_[4];
    EventQueue&  bus_;
    int          debounceMs_;
    bool         activeLow_;


    // per-button debounce state
    bool lastRaw_[4]   = { false,false,false,false }; // last sampled level
    bool stableDown_[4]= { false,false,false,false }; // debounced state
    int  timerMs_[4]   = { 0,0,0,0 };                 // timers for each button
};
