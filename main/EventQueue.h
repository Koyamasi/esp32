#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Event.h"

class EventQueue {
public:
    explicit EventQueue(unsigned capacity = 32);
    bool send(const Event& e);
    bool recv(Event& e, TickType_t toTicks);
    QueueHandle_t handle() const { return q_; }
private:
    QueueHandle_t q_{nullptr};
};
