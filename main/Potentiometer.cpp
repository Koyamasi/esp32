// Potentiometer.cpp - implementation of ADC-based potentiometer reader

#include "Potentiometer.h"
#include "esp_check.h"
#include "esp_log.h"

static const char* TAG = "Potentiometer";

// Initialize the ADC unit/channel and store configuration
Potentiometer::Potentiometer(adc_unit_t unit, adc_channel_t ch, uint8_t id,
                             EventQueue& bus, int reportIntervalMs)
    : unit_(unit), ch_(ch), id_(id), bus_(bus), reportIntervalMs_(reportIntervalMs) {
    adc_oneshot_unit_init_cfg_t init_cfg = {};
    init_cfg.unit_id = unit_;
    init_cfg.ulp_mode = ADC_ULP_MODE_DISABLE;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &unitHandle_));

    adc_oneshot_chan_cfg_t chan_cfg = {};
    chan_cfg.bitwidth = ADC_BITWIDTH_DEFAULT;
    chan_cfg.atten = ADC_ATTEN_DB_12;
    ESP_ERROR_CHECK(adc_oneshot_config_channel(unitHandle_, ch_, &chan_cfg));
}

// Clean up the ADC driver when destroyed
Potentiometer::~Potentiometer() {
    if (unitHandle_) {
        adc_oneshot_del_unit(unitHandle_);
        unitHandle_ = nullptr;
    }
}

// Map a raw 0..4095 reading into 8 buckets numbered 1..8
int Potentiometer::bucket8(int raw) {
    int b = 1 + (raw * 8) / 4096;
    if (b < 1) b = 1;
    if (b > 8) b = 8;
    return b;
}

// Periodically sample the ADC and emit an event when the bucket changes
void Potentiometer::poll(int dtMs) {
    timerMs_ += dtMs;
    if (timerMs_ < reportIntervalMs_) return; // not time yet
    timerMs_ = 0;

    int raw = 0;
    esp_err_t err = adc_oneshot_read(unitHandle_, ch_, &raw);
    if (err == ESP_OK) {
        int b = bucket8(raw);
        if (b != lastBucket_) {
            lastBucket_ = b;
            Event e;
            e.type = EventType::PotBucket;
            e.id = id_;
            e.data.bucket = b;
            bus_.send(e);
        }
    } else {
        ESP_LOGW(TAG, "adc_oneshot_read failed: %s (%d)", esp_err_to_name(err), err);
    }
}
