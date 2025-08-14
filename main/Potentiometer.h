/**
 * @file Potentiometer.h
 * @brief ADC-based potentiometer reader with bucketized events.
 */

#pragma once
#include <stdint.h>
#include "esp_adc/adc_oneshot.h"
#include "EventQueue.h"

/**
 * @brief Reads a potentiometer using the ESP32 one-shot ADC driver and reports
 * the value in coarse "buckets" to reduce serial traffic.
 */
class Potentiometer {
public:
    /**
     * @brief Initialize the potentiometer reader.
     * @param unit ADC unit to use.
     * @param ch ADC channel on the unit.
     * @param id Identifier for generated events.
     * @param bus Event queue to publish to.
     * @param reportIntervalMs Polling period in milliseconds.
     */
    Potentiometer(adc_unit_t unit, adc_channel_t ch, uint8_t id,
                  EventQueue& bus, int reportIntervalMs);
    /**
     * @brief Clean up the ADC driver.
     */
    ~Potentiometer();

    /**
     * @brief Poll the ADC and send an event when the bucket changes.
     * @param dtMs Elapsed time in milliseconds since the last call.
     */
    void poll(int dtMs);

    /**
     * @brief Convert a raw ADC reading into one of eight buckets.
     * @param raw Raw ADC reading.
     * @return Bucket number from 1 to 8.
     */
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
