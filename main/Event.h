// Event.h - simple event structure used for inter-task communication

#pragma once
#include <stdint.h>

// Types of events that may be posted to the EventQueue
enum class EventType : uint8_t {
    ButtonEdge, // A button changed state
    PotBucket   // Potentiometer entered a new bucket
};

// Generic event container.  "id" distinguishes which device generated it
// and the union holds type-specific data.
struct Event {
    EventType type; // What kind of event it is
    uint8_t   id;   // Identifier of the source (button number, etc.)
    union {
        bool down;   // For ButtonEdge: true when pressed
        int  bucket; // For PotBucket: 1..8 bucket number
    } data;
};
