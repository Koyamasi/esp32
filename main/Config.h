// Config.h - central location for firmware constants

#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

namespace cfg {
// Identifiers used when sending events
static const uint8_t BTN1_ID = 1; // single button ID
static const uint8_t POT1_ID = 2; // potentiometer ID
static const uint8_t BTN4_IDS[4] = { 11, 12, 13, 14 }; // four-button block IDs

// GPIO assignments
static const gpio_num_t BTN1_PIN = GPIO_NUM_42; // user push button
static const gpio_num_t LED1_PIN = GPIO_NUM_5;  // indicator LED
static const gpio_num_t BTN4_PINS[4] = {
    GPIO_NUM_41, // B1
    GPIO_NUM_40, // B2
    GPIO_NUM_39, // B3
    GPIO_NUM_38  // B4
};

// Final pin layout:
//   BTN1_PIN (ID 1) -> GPIO42
//   Four-button block (IDs 11-14) -> GPIO41, GPIO40, GPIO39, GPIO38
//   LED1_PIN -> GPIO5

// ADC configuration for the potentiometer
static const adc_unit_t    POT_UNIT = ADC_UNIT_1;
static const adc_channel_t POT_CH   = ADC_CHANNEL_6;

// Timing configuration (in milliseconds)
static const int POLL_PERIOD_MS = 5;  // main loop period
static const int DEBOUNCE_MS    = 30; // button debounce time
static const int POT_INTERVAL_MS= 80; // pot report interval
}
