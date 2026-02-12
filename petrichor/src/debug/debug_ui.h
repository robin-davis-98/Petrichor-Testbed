#pragma once

struct Window;
struct Renderer;

void debug_Init();
void debug_RenderUI(Window* window, Renderer* renderer);
void debug_Shutdown();