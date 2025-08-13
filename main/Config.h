#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

namespace cfg {
static const uint8_t BTN1_ID = 1;
static const uint8_t POT1_ID = 2;

static const gpio_num_t BTN1_PIN = GPIO_NUM_42;
static const gpio_num_t LED1_PIN = GPIO_NUM_5;

static const adc_unit_t    POT_UNIT = ADC_UNIT_1;
static const adc_channel_t POT_CH   = ADC_CHANNEL_6;

static const int POLL_PERIOD_MS = 5;
static const int DEBOUNCE_MS    = 30;
static const int POT_INTERVAL_MS= 80;
}
