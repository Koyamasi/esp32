#pragma once
#include <stdint.h>
#include "esp_adc/adc_oneshot.h"
#include "EventQueue.h"

class Potentiometer {
public:
    Potentiometer(adc_unit_t unit, adc_channel_t ch, uint8_t id, EventQueue& bus, int reportIntervalMs);
    ~Potentiometer();
    void poll(int dtMs);
    static int bucket8(int raw);
private:
    adc_unit_t unit_;
    adc_channel_t ch_;
    uint8_t id_;
    EventQueue& bus_;
    int reportIntervalMs_;
    adc_oneshot_unit_handle_t unitHandle_{nullptr};
    int timerMs_{0};
    int lastBucket_{-1};
};
