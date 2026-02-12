#include "events/event_queue.h"

void event_Init(EventQueue* queue) {
    queue->count = 0;
    queue->head = &queue->queue[0];
    queue->tail = &queue->queue[0];
}

void event_Push(EventQueue* queue, const Event& event) {
    if (queue->count >= MAX_EVENTS) {
        return;
    }

    *queue->tail = event;
    queue->tail++;

    if (queue->tail >= &queue->queue[MAX_EVENTS]) {
        queue->tail = &queue->queue[0];
    }

    queue->count++;
}

Event event_Pop(EventQueue* queue) {
    if (queue->count == 0) return {};

    Event e = *queue->head;
    queue->head++;

    if (queue->head >= &queue->queue[MAX_EVENTS]) {
        queue->head = &queue->queue[0];
    }

    queue->count--;
    return e;
}