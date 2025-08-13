#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "Config.h"
#include "EventQueue.h"
#include "Led.h"
#include "Button.h"
#include "Potentiometer.h"
#include "SerialMessenger.h"

// ADDED
#include "FourButtons.h"
#include "esp_task_wdt.h"   // ADDED: for watchdog control

static const char* TAG = "unity_io_cpp_clean";

extern "C" void app_main(void) {
    // ADDED: stop Task WDT from nagging about IDLE0 while we're hammering prints
    esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0));
    // If it ever complains about IDLE1 too, uncomment this:
    // esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(1));

    ESP_LOGI(TAG, "Starting (C++) input firmware");

    static EventQueue bus(32);

    static Led led(cfg::LED1_PIN);
    static Button btn(cfg::BTN1_PIN, cfg::BTN1_ID, bus, cfg::DEBOUNCE_MS, &led);
    static Potentiometer pot(cfg::POT_UNIT, cfg::POT_CH, cfg::POT1_ID, bus, cfg::POT_INTERVAL_MS);

    static SerialMessenger tx(bus);
    tx.startTask("serial_tx", 4096, 1);  // keep low prio

    // 4-button block (active-low with internal pull-ups)
    static const ButtonSpec four_specs[4] = {
        { GPIO_NUM_42, 11 },
        { GPIO_NUM_41, 12 },
        { GPIO_NUM_40, 13 },
        { GPIO_NUM_39, 14 },
    };
    static FourButtons four(four_specs, /*debounceMs=*/30, /*activeLow=*/true);

    ESP_LOGI(TAG, "Polling every %d ms; UART @ 115200", cfg::POLL_PERIOD_MS);
    ESP_LOGI(TAG, "Messages: BTN,<id>,DOWN/UP | POT,<id>,1..8");

    // single dt + period (no duplicates)
    const int dt_ms = cfg::POLL_PERIOD_MS;
    const TickType_t period = pdMS_TO_TICKS(dt_ms);

    // throttle pot calls additionally (cuts noise/flooding)
    int pot_accum_ms = 0;

    while (true) {
        btn.poll(dt_ms);
        four.poll(dt_ms);

        pot_accum_ms += dt_ms;
        if (pot_accum_ms >= cfg::POT_INTERVAL_MS) {
            pot.poll(pot_accum_ms);   // pass the elapsed time chunk
            pot_accum_ms = 0;
        }

        vTaskDelay(period);
    }
}
