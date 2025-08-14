// EventQueue.cpp - implementation of Event queue wrapper

#include "EventQueue.h"

// Create a FreeRTOS queue capable of holding the requested number of events
EventQueue::EventQueue(unsigned capacity) {
    q_ = xQueueCreate(capacity, sizeof(Event));
}

// Post an event to the queue without waiting.  Returns false if the queue
// does not exist or is full.
bool EventQueue::send(const Event& e) {
    if (!q_) return false;
    return xQueueSend(q_, &e, 0) == pdTRUE;
}

// Receive an event, waiting up to "toTicks" ticks.  Returns false if the queue
// does not exist or a timeout occurs.
bool EventQueue::recv(Event& e, TickType_t toTicks) {
    if (!q_) return false;
    return xQueueReceive(q_, &e, toTicks) == pdTRUE;
}
