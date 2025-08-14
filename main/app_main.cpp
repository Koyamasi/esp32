#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "Config.h"
#include "EventQueue.h"
#include "Led.h"
#include "Button.h"
#include "Potentiometer.h"
#include "SerialMessenger.h"

// Extra helper for reading four additional buttons
#include "FourButtons.h"
#include "esp_task_wdt.h"   // ADDED: for watchdog control

// Tag used for ESP_LOG output
static const char* TAG = "unity_io_cpp_clean";

// Entry point called by ESP-IDF after initialization
extern "C" void app_main(void) {
    // ADDED: stop Task WDT from nagging about IDLE0 while we're hammering prints
    esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0));
    // If it ever complains about IDLE1 too, uncomment this:
    // esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(1));

    ESP_LOGI(TAG, "Starting (C++) input firmware");

    // Central message bus for passing events between components
    static EventQueue bus(32);

    // Hardware components
    static Led led(cfg::LED1_PIN);
    static Button btn(cfg::BTN1_PIN, cfg::BTN1_ID, bus,
                      cfg::DEBOUNCE_MS, &led);
    static Potentiometer pot(cfg::POT_UNIT, cfg::POT_CH, cfg::POT1_ID,
                             bus, cfg::POT_INTERVAL_MS);

    // Task that prints events to the serial port
    static SerialMessenger tx(bus);
    tx.startTask("serial_tx", 4096, 1);  // keep low prio


    // Four additional buttons (active-low with internal pull-ups).
    // Pins avoid cfg::BTN1_PIN and map to IDs 11-14 as shown below:
    //   GPIO41->ID11, GPIO40->ID12, GPIO39->ID13, GPIO38->ID14
    static const ButtonSpec four_specs[4] = {
        { cfg::BTN4_PINS[0], cfg::BTN4_IDS[0] },
        { cfg::BTN4_PINS[1], cfg::BTN4_IDS[1] },
        { cfg::BTN4_PINS[2], cfg::BTN4_IDS[2] },
        { cfg::BTN4_PINS[3], cfg::BTN4_IDS[3] },
    };
    static FourButtons four(four_specs, bus, /*debounceMs=*/30, /*activeLow=*/true);

    ESP_LOGI(TAG, "Polling every %d ms; UART @ 115200", cfg::POLL_PERIOD_MS);
    ESP_LOGI(TAG, "Messages: BTN,<id>,DOWN/UP | POT,<id>,1..8");


    // Main loop periodically polls inputs and sleeps between iterations
    // Polling interval in milliseconds and RTOS ticks
    const int dt_ms = cfg::POLL_PERIOD_MS;
    const TickType_t period = pdMS_TO_TICKS(dt_ms);

    // throttle pot calls additionally (cuts noise/flooding)
    int pot_accum_ms = 0;

    while (true) {
        btn.poll(dt_ms);
        // poll the 4-button block (events BTN,11..14,DOWN/UP)
        four.poll(dt_ms);
        pot_accum_ms += dt_ms;
        if (pot_accum_ms >= cfg::POT_INTERVAL_MS) {
            pot.poll(pot_accum_ms);   // pass the elapsed time chunk
            pot_accum_ms = 0;
        }

        vTaskDelay(period);
    }
}
