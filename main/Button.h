/**
 * @file Button.h
 * @brief Declaration of a debounced push button helper.
 *
 * Each Button instance monitors a GPIO input and posts an event to an
 * EventQueue whenever the button state changes. An optional LED can be
 * "mirrored" so that the LED follows the button state. The implementation
 * performs simple software debouncing based on a poll interval provided by
 * the caller.
 */

#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "EventQueue.h"

/** Forward declaration to avoid circular include. */
class Led;

/** @brief Represents a single debounced button connected to a GPIO pin. */
class Button {
public:
    /**
     * @brief Construct a button on the given pin.
     * @param pin GPIO pin connected to the button.
     * @param id Identifier used when generating events.
     * @param bus Queue to which events are posted.
     * @param debounceMs Minimum time a level must be stable before reporting.
     * @param mirror Optional LED whose state mirrors the button.
     */
    Button(gpio_num_t pin, uint8_t id, EventQueue& bus, int debounceMs,
           Led* mirror = nullptr);

    /**
     * @brief Poll the hardware.
     * @param dtMs Elapsed time in milliseconds since the last call.
     */
    void poll(int dtMs);

private:
    // Configuration established at construction time
    gpio_num_t  pin_;        // GPIO pin connected to the button
    uint8_t     id_;         // Identifier used when sending events
    EventQueue& bus_;        // Queue to publish events to
    int         debounceMs_; // Debounce interval in milliseconds
    Led*        mirror_;     // Optional LED that mirrors the state

    // Debounce state
    bool lastRaw_{false};    // Last raw sampled level
    bool stableDown_{false}; // Last debounced state
    int  timerMs_{0};        // Time accumulator for debouncing
};
