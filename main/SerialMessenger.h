#pragma once
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "EventQueue.h"

class SerialMessenger {
public:
    explicit SerialMessenger(EventQueue& bus);
    void startTask(const char* name = "serial_tx", uint32_t stack = 4096, UBaseType_t prio = 5);
private:
    static void taskTrampoline(void* arg);
    void run();
    EventQueue& bus_;
};
