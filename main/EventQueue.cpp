#include "EventQueue.h"

EventQueue::EventQueue(unsigned capacity) {
    q_ = xQueueCreate(capacity, sizeof(Event));
}

bool EventQueue::send(const Event& e) {
    if (!q_) return false;
    return xQueueSend(q_, &e, 0) == pdTRUE;
}

bool EventQueue::recv(Event& e, TickType_t toTicks) {
    if (!q_) return false;
    return xQueueReceive(q_, &e, toTicks) == pdTRUE;
}
