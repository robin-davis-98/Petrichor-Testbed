#include "graphics/renderer.h"

#include <cassert>

#include "renderer/opengl/renderer_opengl.h"

bool renderer_Init(Renderer* renderer, RendererBackend backend, void* window_handle) {
    renderer->backend = backend;

    switch (backend) {
        case RENDERER_BACKEND_OPENGL:
            return opengl_backend_init(renderer, window_handle);
        default:
            assert("RENDERER BACKEND NOT IMPLEMENTED");
            break;
    }
    return false;
}

void renderer_BeginFrame(Renderer* renderer) {
    switch (renderer->backend) {
        case RENDERER_BACKEND_OPENGL: opengl_backend_begin_frame(renderer);
        default:
            assert("RENDERER BACKEND NOT IMPLEMENTED");
            break;
    }
}

void renderer_EndFrame(Renderer* renderer) {
    switch (renderer->backend) {
        case RENDERER_BACKEND_OPENGL: opengl_backend_end_frame(renderer);
        default:
            assert("RENDERER BACKEND NOT IMPLEMENTED");
            break;
    }
}

void renderer_Resize(Renderer* renderer, uint32_t width, uint32_t height) {
    switch (renderer->backend) {
        case RENDERER_BACKEND_OPENGL: opengl_backend_resize(renderer, width, height); break;
        default:
            assert("RENDERER BACKEND NOT IMPLEMENTED");
            break;
    }
}

void renderer_Clear(Renderer *renderer, float r, float g, float b, float a) {
    switch (renderer->backend) {
        case RENDERER_BACKEND_OPENGL: opengl_backend_clear(renderer, r, g, b, a); break;
        default:
            assert("RENDERER BACKEND NOT IMPLEMENTED");
            break;
    }
}

void renderer_Present(Renderer *renderer) {
    switch (renderer->backend) {
        case RENDERER_BACKEND_OPENGL: opengl_backend_present(renderer); break;
        default:
            assert("RENDERER BACKEND NOT IMPLEMENTED");
            break;
    }
}