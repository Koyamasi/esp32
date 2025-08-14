// SerialMessenger.cpp - print events from the queue to the serial port

#include "SerialMessenger.h"

SerialMessenger::SerialMessenger(EventQueue& bus) : bus_(bus) {
    // Flush each line immediately so that test harnesses see events promptly
    setvbuf(stdout, NULL, _IONBF, 0);
}

// Start the FreeRTOS task that will process events
void SerialMessenger::startTask(const char* name, uint32_t stack, UBaseType_t prio) {
    xTaskCreate(&SerialMessenger::taskTrampoline, name, stack, this, prio, nullptr);
}

// Static wrapper that invokes the member function
void SerialMessenger::taskTrampoline(void* arg) {
    static_cast<SerialMessenger*>(arg)->run();
}

// Main task loop: wait for events and print them
void SerialMessenger::run() {
    Event e;
    while (true) {
        if (bus_.recv(e, portMAX_DELAY)) {
            switch (e.type) {
            case EventType::ButtonEdge:
                printf("BTN,%u,%s\n", e.id, e.data.down ? "DOWN" : "UP");
                break;
            case EventType::PotBucket:
                printf("POT,%u,%d\n", e.id, e.data.bucket);
                break;
            }
            // Small delay gives other tasks a chance to run and avoids WDT
            vTaskDelay(1);
        }
    }
}

