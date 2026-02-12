#pragma once

#include "engine.h"
#include "graphics/window.h"
#include "graphics/renderer.h"
#include "events/event_queue.h"

struct ENGINE_API App {
    Window window;
    Renderer renderer;
    EventQueue eventQueue;

    bool running;
};

ENGINE_API App app_Create();
ENGINE_API void app_Run(App& app);