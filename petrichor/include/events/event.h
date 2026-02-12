#pragma once

#include <cstdint>

enum EventType {
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,

    EVENT_MOUSE_BUTTON_DOWN,
    EVENT_MOUSE_BUTTON_UP,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_WHEEL,

    EVENT_WINDOW_CLOSE,
    EVENT_WINDOW_RESIZE,
    EVENT_WINDOW_FULLSCREEN_CHANGED,

    EVENT_WINDOW_SET_FULLSCREEN,
    EVENT_WINDOW_SET_BORDERLESS,
    EVENT_WINDOW_SET_VISIBILITY,

};

struct Event {
    EventType type;
    alignas(8) uint8_t data[16];
};

struct KeyEventData {

};

struct MouseMoveData {

};

struct MouseClickData {

};

struct WindowStatusData {
    uint32_t width, height;
    bool border;
    bool fullscreen;
    bool visible;
};
