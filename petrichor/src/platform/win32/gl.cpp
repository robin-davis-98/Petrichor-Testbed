#include "platform/platform.h"
#include "platform/win32/win32_internal.h"
#include "renderer/opengl/gl_internal.h"
#include <windows.h>

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

bool opengl_CreateContext(Window* window, GL_Renderer_Context* out_context) {
    auto* win32_window = reinterpret_cast<Win32_Window_Data*>(window->handle.data);
    HDC hdc = GetDC(win32_window->handle);

    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1 };
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    HGLRC tempContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempContext);

    auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
        wglGetProcAddress("wglCreateContextAttribsARB");

    int attribs[] = {
        0x2091, 4,           // WGL_CONTEXT_MAJOR_VERSION_ARB
        0x2092, 3,           // WGL_CONTEXT_MINOR_VERSION_ARB
        0x9126, 0x00000001,  // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB
        0                    // End
    };

    HGLRC hglrc = wglCreateContextAttribsARB(hdc, NULL, attribs);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);

    wglMakeCurrent(hdc, hglrc);

    out_context->deviceContext = (void*)hdc;
    out_context->renderContext = (void*)hglrc;

    return (hglrc != nullptr);
}

void opengl_MakeCurrent(GL_Renderer_Context* context) {
    wglMakeCurrent((HDC)context->deviceContext, (HGLRC)context->renderContext);
}

void opengl_SwapBuffers(GL_Renderer_Context* context) {
    SwapBuffers((HDC)context->deviceContext);
}

void opengl_DestroyContext(GL_Renderer_Context* context) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext((HGLRC)context->renderContext);
}

void* opengl_GetProcAddress(const char* name) {
    void* p = (void*)wglGetProcAddress(name);
    if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, name);
    }
    return p;
}