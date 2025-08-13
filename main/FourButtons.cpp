#include "FourButtons.h"
#include <stdio.h>

FourButtons::FourButtons(const ButtonSpec specs[4], int debounceMs, bool activeLow)
    : debounceMs_(debounceMs), activeLow_(activeLow) {
    for (int i = 0; i < 4; ++i) {
        btn_[i] = specs[i];

        // Configure as input with internal biasing
        gpio_config_t cfg = {};
        cfg.pin_bit_mask = 1ULL << btn_[i].pin;
        cfg.mode = GPIO_MODE_INPUT;
        cfg.pull_up_en = activeLow ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = activeLow ? GPIO_PULLDOWN_DISABLE : GPIO_PULLDOWN_ENABLE;
        cfg.intr_type = GPIO_INTR_DISABLE;
        gpio_config(&cfg);

        // Initialize lastRaw from current level
        bool raw = gpio_get_level(btn_[i].pin);
        lastRaw_[i] = activeLow ? (raw == 0) : (raw != 0); // store "pressed?" interpretation
        stableDown_[i] = lastRaw_[i];
    }
}

void FourButtons::poll(int dtMs) {
    for (int i = 0; i < 4; ++i) {
        bool rawLevel = gpio_get_level(btn_[i].pin);
        bool rawDown = activeLow_ ? (rawLevel == 0) : (rawLevel != 0);

        if (rawDown != lastRaw_[i]) {
            lastRaw_[i] = rawDown;
            timerMs_[i] = 0;            // restart debounce window
            continue;
        }

        if (timerMs_[i] < debounceMs_) {
            timerMs_[i] += dtMs;
            if (timerMs_[i] >= debounceMs_ && rawDown != stableDown_[i]) {
                stableDown_[i] = rawDown;
                // Print exactly one line per stable edge
                printf("BTN,%u,%s\n", btn_[i].id, stableDown_[i] ? "DOWN" : "UP");
            }
        }
    }
}
