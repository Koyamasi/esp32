/**
 * @file FourButtons.h
 * @brief Helper for reading four debounced buttons.
 */

#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "EventQueue.h"

/** @brief One struct per button specifying the GPIO pin and event ID. */
struct ButtonSpec {
    gpio_num_t pin; ///< GPIO pin the button is connected to
    uint8_t    id;  ///< Identifier printed when the button changes state
};

/**
 * @brief Debounced four-button reader.
 *
 * Active-LOW by default (internal pull-ups on). The class polls all four
 * buttons and prints a line on the UART whenever a stable edge is detected.
 */
class FourButtons {
public:
    /**
     * @brief Construct with button specifications and event queue.
     * @param specs Array of four button specifications.
     * @param bus Event queue where button events are sent.
     * @param debounceMs Minimum stable time in milliseconds to accept an edge.
     * @param activeLow True if buttons are active-low.
     */
    FourButtons(const ButtonSpec specs[4], EventQueue& bus, int debounceMs, bool activeLow = true);


    /**
     * @brief Poll all buttons and send events on edges.
     * @param dtMs Elapsed time in milliseconds since the last call.
     */
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
