#pragma once
#include <stdint.h>

enum class EventType : uint8_t {
    ButtonEdge,
    PotBucket
};

struct Event {
    EventType type;
    uint8_t   id;
    union {
        bool down;
        int  bucket;
    } data;
};
