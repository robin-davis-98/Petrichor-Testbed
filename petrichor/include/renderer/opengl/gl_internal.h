#pragma once
#include "platform/platform.h"
#include <glad/glad.h>

struct GL_Renderer_Context {
    void* deviceContext;
    void* renderContext;
};

bool opengl_CreateContext(Window* window, GL_Renderer_Context* out_context);
void opengl_DestroyContext(GL_Renderer_Context* context);
void opengl_MakeCurrent(GL_Renderer_Context* context);
void opengl_SwapBuffers(GL_Renderer_Context* context);
void* opengl_GetProcAddress(const char* name);
