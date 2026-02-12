#include "graphics/renderer.h"

#include "glad/glad.h"
#include "renderer/opengl/gl_internal.h"

bool renderer_Init(Renderer* renderer, RendererBackend backend, void* window_handle) {
    renderer->backend = backend;
    Window* window = static_cast<Window*>(window_handle);

    if (backend == RENDERER_BACKEND_OPENGL) {
        // Map our internal byte array to the OpenGL context struct
        auto* context = reinterpret_cast<GL_Renderer_Context*>(renderer->internal_state);

        // 1. Call the Win32/Platform function to create the 4.3 context
        if (!opengl_CreateContext(window, context)) {
            return false;
        }

        // 2. Load GLAD (now that a context is current)
        if (!gladLoadGLLoader((GLADloadproc)opengl_GetProcAddress)) {
            return false;
        }

        return true;
    }
    return false;
}

void renderer_Resize(Renderer* renderer, uint32_t width, uint32_t height) {
    if (renderer->backend == RENDERER_BACKEND_OPENGL) {
        glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    }
}

void renderer_Clear(Renderer *renderer, float r, float g, float b, float a) {
    if (renderer->backend == RENDERER_BACKEND_OPENGL) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void renderer_Present(Renderer *renderer) {
    if (renderer->backend == RENDERER_BACKEND_OPENGL) {
        auto* context = reinterpret_cast<GL_Renderer_Context*>(renderer->internal_state);
        opengl_SwapBuffers(context);
    }
}