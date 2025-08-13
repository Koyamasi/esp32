#include "Potentiometer.h"
#include "esp_check.h"

Potentiometer::Potentiometer(adc_unit_t unit, adc_channel_t ch, uint8_t id, EventQueue& bus, int reportIntervalMs)
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

Potentiometer::~Potentiometer() {
    if (unitHandle_) {
        adc_oneshot_del_unit(unitHandle_);
        unitHandle_ = nullptr;
    }
}

int Potentiometer::bucket8(int raw) {
    int b = 1 + (raw * 8) / 4096;
    if (b < 1) b = 1;
    if (b > 8) b = 8;
    return b;
}

void Potentiometer::poll(int dtMs) {
    timerMs_ += dtMs;
    if (timerMs_ < reportIntervalMs_) return;
    timerMs_ = 0;

    int raw = 0;
    if (adc_oneshot_read(unitHandle_, ch_, &raw) == ESP_OK) {
        int b = bucket8(raw);
        if (b != lastBucket_) {
            lastBucket_ = b;
            Event e;
            e.type = EventType::PotBucket;
            e.id = id_;
            e.data.bucket = b;
            bus_.send(e);
        }
    }
}
