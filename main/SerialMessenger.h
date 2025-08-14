/**
 * @file SerialMessenger.h
 * @brief Task that forwards events to the UART.
 */

#pragma once
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "EventQueue.h"

/**
 * @brief Consumes events from an EventQueue and prints them to stdout.
 * Useful for debugging when connected to a serial terminal.
 */
class SerialMessenger {
public:
    /**
     * @brief Construct with a reference to the EventQueue to listen on.
     * @param bus EventQueue providing events.
     */
    explicit SerialMessenger(EventQueue& bus);

    /**
     * @brief Launch the FreeRTOS task that waits for events and prints them.
     * @param name Task name.
     * @param stack Stack size in bytes.
     * @param prio Task priority.
     */
    void startTask(const char* name = "serial_tx", uint32_t stack = 4096,
                   UBaseType_t prio = 5);

private:
    /**
     * @brief Static trampoline used by xTaskCreate.
     * @param arg Pointer to SerialMessenger instance.
     */
    static void taskTrampoline(void* arg);

    /**
     * @brief Main task body: receive and print events.
     */
    void run();

    EventQueue& bus_; // queue from which events are read
};
