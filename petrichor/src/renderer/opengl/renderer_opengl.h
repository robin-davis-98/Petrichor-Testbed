#pragma once

#include <cstdint>

struct Renderer;

bool opengl_backend_init(Renderer* renderer, void* window_handle);
void opengl_backend_clear(Renderer* renderer, float r, float g, float b, float a);
void opengl_backend_begin_frame(Renderer* renderer);
void opengl_backend_end_frame(Renderer* renderer);
void opengl_backend_present(Renderer* renderer);
void opengl_backend_resize(Renderer* renderer, uint32_t width, uint32_t height);