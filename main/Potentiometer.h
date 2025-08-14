// Potentiometer.h - ADC-based potentiometer reader with bucketized events

#pragma once
#include <stdint.h>
#include "esp_adc/adc_oneshot.h"
#include "EventQueue.h"

// Reads a potentiometer using the ESP32 one-shot ADC driver and reports the
// value in coarse "buckets" to reduce serial traffic.
class Potentiometer {
public:
    // "unit" and "ch" select the ADC unit/channel.  "id" identifies events
    // sent to the EventQueue.  "reportIntervalMs" is the polling period.
    Potentiometer(adc_unit_t unit, adc_channel_t ch, uint8_t id,
                  EventQueue& bus, int reportIntervalMs);
    ~Potentiometer();

    // Poll the ADC and send an event when the bucket changes
    void poll(int dtMs);

    // Convert a raw ADC reading into one of eight buckets (1..8)
    static int bucket8(int raw);

private:
    adc_unit_t unit_;   // ADC unit being used
    adc_channel_t ch_;  // Channel on the ADC unit
    uint8_t id_;        // Identifier for events
    EventQueue& bus_;   // Destination for events
    int reportIntervalMs_; // Minimum interval between reports
    adc_oneshot_unit_handle_t unitHandle_{nullptr}; // ADC driver handle
    int timerMs_{0};    // Accumulator for interval timing
    int lastBucket_{-1}; // Previously reported bucket
};
