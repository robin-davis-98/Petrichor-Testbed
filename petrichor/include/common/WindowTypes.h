#pragma once

#include <cstdint>

struct Window_Handle{
    alignas(8) uint8_t data[32];
};

enum WindowFlags : uint8_t {
    WINDOW_FLAG_RESIZABLE       = 1 << 0,
    WINDOW_FLAG_BORDERLESS      = 1 << 1,
    WINDOW_FLAG_FULLSCREEN      = 1 << 2,
    WINDOW_FLAG_HIDDEN          = 1 << 3,
};

struct WindowProperties {
    uint32_t width, height; // Physical Window Size

    uint32_t windowed_width, windowed_height; // Window Size outside Fullscreen / Fullscreen Borderless

    uint32_t internal_resolution_width, internal_resolution_height; // Base Target Resolution
    uint32_t render_resolution_width, render_resolution_height; // The resolution the GPU is targeting
    uint32_t flags;
};

struct Window {
    WindowProperties properties;
    Window_Handle handle;
};