#include "Led.h"

Led::Led(gpio_num_t pin) : pin_(pin) {
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = 1ULL << pin_;
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);
    set(false);
}

void Led::set(bool on) {
    gpio_set_level(pin_, on ? 1 : 0);
}

void Led::toggle() {
    set(!get());
}

bool Led::get() const {
    return gpio_get_level(pin_) != 0;
}
