// EventQueue.h - thin wrapper around a FreeRTOS queue for Event objects

#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Event.h"

// Provides send/receive helpers with type safety for Event structures
class EventQueue {
public:
    // Create a queue capable of holding "capacity" events
    explicit EventQueue(unsigned capacity = 32);

    // Send an event to the queue.  Returns true on success.
    bool send(const Event& e);

    // Receive an event from the queue, waiting up to "toTicks" ticks.
    // Returns true if an event was received.
    bool recv(Event& e, TickType_t toTicks);

    // Access the underlying FreeRTOS queue handle
    QueueHandle_t handle() const { return q_; }

private:
    QueueHandle_t q_{nullptr}; // FreeRTOS queue handle
};
