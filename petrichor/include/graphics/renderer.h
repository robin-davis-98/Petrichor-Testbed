#pragma once
#include <cstdint>

enum RendererBackend {
    RENDERER_BACKEND_OPENGL,
    RENDERER_BACKEND_VULKAN,
    RENDERER_BACKEND_DX11,
    RENDERER_BACKEND_DX12,
};

struct Renderer {
    RendererBackend backend;
    alignas(8) uint8_t internal_state[1024];
};

bool renderer_Init(Renderer* renderer, RendererBackend backend, void* window_handle);
void renderer_Clear(Renderer* renderer, float r, float g, float b, float a);
void renderer_Resize(Renderer* renderer, uint32_t width, uint32_t height);
void renderer_Present(Renderer* renderer);