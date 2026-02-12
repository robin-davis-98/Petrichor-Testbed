#pragma once

#include <cstdint>
#include "event.h"

#define MAX_EVENTS 1024

struct EventQueue {
    Event queue[MAX_EVENTS];
    Event* head;
    Event* tail;
    uint32_t count;
};

void event_Init(EventQueue* queue);
void event_Push(EventQueue* queue, const Event& event);
Event event_Pop(EventQueue* queue);