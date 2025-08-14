/**
 * @file EventQueue.h
 * @brief Thin wrapper around a FreeRTOS queue for Event objects.
 */

#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Event.h"

/** @brief Provides send/receive helpers with type safety for Event structures. */
class EventQueue {
public:
    /**
     * @brief Create a queue capable of holding a number of events.
     * @param capacity Maximum number of events the queue can hold.
     */
    explicit EventQueue(unsigned capacity = 32);

    /**
     * @brief Send an event to the queue.
     * @param e Event to send.
     * @return true on success.
     */
    bool send(const Event& e);

    /**
     * @brief Receive an event from the queue.
     * @param e Event object to populate.
     * @param toTicks Maximum number of ticks to wait.
     * @return true if an event was received.
     */
    bool recv(Event& e, TickType_t toTicks);

    /**
     * @brief Access the underlying FreeRTOS queue handle.
     * @return Native queue handle.
     */
    QueueHandle_t handle() const { return q_; }

private:
    QueueHandle_t q_{nullptr}; // FreeRTOS queue handle
};
