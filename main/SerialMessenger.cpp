#include "SerialMessenger.h"

SerialMessenger::SerialMessenger(EventQueue& bus) : bus_(bus) {
    // flush each line immediately so Unity sees events instantly
    setvbuf(stdout, NULL, _IONBF, 0);
}

void SerialMessenger::startTask(const char* name, uint32_t stack, UBaseType_t prio) {
    xTaskCreate(&SerialMessenger::taskTrampoline, name, stack, this, prio, nullptr);
}

void SerialMessenger::taskTrampoline(void* arg) {
    static_cast<SerialMessenger*>(arg)->run();
}

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
            vTaskDelay(1);   // <-- give IDLE a chance; prevents WDT
        }
    }
}

