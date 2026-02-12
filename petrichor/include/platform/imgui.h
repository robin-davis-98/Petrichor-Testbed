#pragma once

struct Window;

bool platform_imgui_init(Window* window);
void platform_imgui_new_frame();
void platform_imgui_shutdown();