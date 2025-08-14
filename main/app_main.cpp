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

// Tag used for ESP_LOG output
static const char* TAG = "unity_io_cpp_clean";

// Entry point called by ESP-IDF after initialization
extern "C" void app_main(void) {
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
    tx.startTask("serial_tx", 4096, 1);   // was 5; priority 1 is fine

    // Four additional buttons (active-low with internal pull-ups).  IDs are
    // chosen to avoid collision with existing BTN1_ID.
    static const ButtonSpec four_specs[4] = {
        { GPIO_NUM_42, 11 },
        { GPIO_NUM_41, 12 },
        { GPIO_NUM_40, 13 },
        { GPIO_NUM_39, 14 },
    };
    static FourButtons four(four_specs, /*debounceMs=*/30, /*activeLow=*/true);

    ESP_LOGI(TAG, "Polling every %d ms; UART @ 115200", cfg::POLL_PERIOD_MS);
    ESP_LOGI(TAG, "Messages: BTN,<id>,DOWN/UP | POT,<id>,1..8");

    // Main loop periodically polls inputs and sleeps between iterations
    const TickType_t period = pdMS_TO_TICKS(cfg::POLL_PERIOD_MS);
    while (true) {
        btn.poll(cfg::POLL_PERIOD_MS);
        pot.poll(cfg::POLL_PERIOD_MS);

        // Poll the additional button block (prints BTN,11..14,DOWN/UP)
        four.poll(cfg::POLL_PERIOD_MS);

        vTaskDelay(period);
    }
}
