#pragma once

#include "common/WindowTypes.h"
#include "events/event_queue.h"
#include <cstdint>

#ifndef APP_NAME
    #define APP_NAME "Petrichor Engine"
#endif

void window_Create(Window* window, EventQueue* queue);

void window_Show(Window* window);
void window_Hide(Window* window);
void window_ToggleFullscreen(Window* window);
void window_ToggleBorder(Window* window);

void window_SetFlags(Window* window, uint32_t flags);

void window_Update(Window* window, EventQueue& queue);

void* window_GetNativeHandle(const Window* window);
