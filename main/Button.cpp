#include "Button.h"
#include "Led.h"
#include "driver/gpio.h"

Button::Button(gpio_num_t pin, uint8_t id, EventQueue& bus, int debounceMs, Led* mirror)
: pin_(pin), id_(id), bus_(bus), debounceMs_(debounceMs), mirror_(mirror) {
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = 1ULL << pin_;
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_ENABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);
}

void Button::poll(int dtMs) {
    bool rawDown = (gpio_get_level(pin_) == 0);
    if (rawDown != lastRaw_) {
        lastRaw_ = rawDown;
        timerMs_ = 0;
        return;
    }
    if (timerMs_ < debounceMs_) {
        timerMs_ += dtMs;
        if (timerMs_ >= debounceMs_ && rawDown != stableDown_) {
            stableDown_ = rawDown;
            if (mirror_) mirror_->set(stableDown_);
            Event e;
            e.type = EventType::ButtonEdge;
            e.id = id_;
            e.data.down = stableDown_;
            bus_.send(e);
        }
    }
}
