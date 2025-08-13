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

static const char* TAG = "unity_io_cpp_clean";

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting (C++) input firmware");

    static EventQueue bus(32);

    static Led led(cfg::LED1_PIN);
    static Button btn(cfg::BTN1_PIN, cfg::BTN1_ID, bus, cfg::DEBOUNCE_MS, &led);
    static Potentiometer pot(cfg::POT_UNIT, cfg::POT_CH, cfg::POT1_ID, bus, cfg::POT_INTERVAL_MS);

    static SerialMessenger tx(bus);
    tx.startTask("serial_tx", 4096, 1);   // was 5; priority 1 is fine

    // ADDED: 4-button block (active-low with internal pull-ups)
    // IDs chosen to avoid collision with existing BTN1_ID
    static const ButtonSpec four_specs[4] = {
        { GPIO_NUM_42, 11 },
        { GPIO_NUM_41, 12 },
        { GPIO_NUM_40, 13 },
        { GPIO_NUM_39, 14 },
    };
    static FourButtons four(four_specs, /*debounceMs=*/30, /*activeLow=*/true);

    ESP_LOGI(TAG, "Polling every %d ms; UART @ 115200", cfg::POLL_PERIOD_MS);
    ESP_LOGI(TAG, "Messages: BTN,<id>,DOWN/UP | POT,<id>,1..8");

    const TickType_t period = pdMS_TO_TICKS(cfg::POLL_PERIOD_MS);
    while (true) {
        btn.poll(cfg::POLL_PERIOD_MS);
        pot.poll(cfg::POLL_PERIOD_MS);

        // ADDED: poll the 4 buttons (prints BTN,11..14,DOWN/UP)
        four.poll(cfg::POLL_PERIOD_MS);

        vTaskDelay(period);
    }


}
