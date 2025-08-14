// Button.cpp - implementation of debounced button helper

#include "Button.h"
#include "Led.h"
#include "driver/gpio.h"

// Configure the GPIO pin for input and optional LED mirror
Button::Button(gpio_num_t pin, uint8_t id, EventQueue& bus, int debounceMs,
               Led* mirror)
    : pin_(pin), id_(id), bus_(bus), debounceMs_(debounceMs), mirror_(mirror) {
    // Basic input configuration with internal pull-up enabled
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = 1ULL << pin_;
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_ENABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);
}

// Poll the button and emit an event when a stable edge is detected
void Button::poll(int dtMs) {
    // Sample the raw level.  The pin is active-low so "0" means pressed.
    bool rawDown = (gpio_get_level(pin_) == 0);

    // Detect transitions and restart the debounce timer on any change
    if (rawDown != lastRaw_) {
        lastRaw_ = rawDown;
        timerMs_ = 0;
        return;                     // wait for a stable level
    }

    // Once the level has been stable long enough, update state and notify
    if (timerMs_ < debounceMs_) {
        timerMs_ += dtMs;
        if (timerMs_ >= debounceMs_ && rawDown != stableDown_) {
            stableDown_ = rawDown;   // debounced state has changed
            if (mirror_) mirror_->set(stableDown_); // optional LED follow

            // Publish event describing the new state
            Event e;
            e.type = EventType::ButtonEdge;
            e.id = id_;
            e.data.down = stableDown_;
            bus_.send(e);
        }
    }
}
