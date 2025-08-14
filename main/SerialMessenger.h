// SerialMessenger.h - task that forwards events to the UART

#pragma once
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "EventQueue.h"

// Consumes events from an EventQueue and prints them to stdout.
// Useful for debugging when connected to a serial terminal.
class SerialMessenger {
public:
    // Construct with a reference to the EventQueue to listen on
    explicit SerialMessenger(EventQueue& bus);

    // Launch the FreeRTOS task that waits for events and prints them
    void startTask(const char* name = "serial_tx", uint32_t stack = 4096,
                   UBaseType_t prio = 5);

private:
    // Static trampoline used by xTaskCreate
    static void taskTrampoline(void* arg);

    // Main task body: receive and print events
    void run();

    EventQueue& bus_; // queue from which events are read
};
