#include "platform/platform.h"
#include "platform/win32/win32_internal.h"
#include "renderer/opengl/gl_internal.h"

#include <imgui_impl_win32.h>
#include "platform/imgui.h"
#include <windows.h>

#include "imgui_impl_opengl3.h"

#include <stdio.h>

struct ViewportDataWin32 {
    HDC hdc;
    HGLRC hglrc;
};

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

bool opengl_CreateContext(Window* window, GL_Renderer_Context* out_context) {
    auto* win32_window = reinterpret_cast<Win32_Window_Data*>(window->handle.data);
    HDC hdc = GetDC(win32_window->handle);

    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1 };
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 0; // Ensure we have alpha bits for transparency/composition
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    HGLRC tempContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempContext);

    auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
    wglGetProcAddress("wglCreateContextAttribsARB");

    int attribs[] = {
        0x2091, 4,           // WGL_CONTEXT_MAJOR_VERSION_ARB
        0x2092, 3,           // WGL_CONTEXT_MINOR_VERSION_ARB
        0x9126, 0x00000002,  // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB
        0                    // End
    };

    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);

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


void platform_imgui_new_frame() {
    ImGui_ImplWin32_NewFrame();
}

void platform_imgui_shutdown() {
    ImGui_ImplWin32_Shutdown();
}

void opengl_PlatformRenderViewports() {
    HDC backup_hdc = wglGetCurrentDC();
    HGLRC backup_hglrc = wglGetCurrentContext();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    // RESTORE IMMEDIATELY
    wglMakeCurrent(backup_hdc, backup_hglrc);
}

void win32_ImGui_Renderer_CreateWindow(ImGuiViewport* vp) {
    ViewportDataWin32* data = new ViewportDataWin32();
    HWND hwnd = (HWND)vp->PlatformHandle;
    data->hdc = GetDC(hwnd);

    // Setup Pixel Format (Matches your main window)
    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1 };
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    int pf = ChoosePixelFormat(data->hdc, &pfd);
    SetPixelFormat(data->hdc, pf, &pfd);

    HGLRC main_hglrc = wglGetCurrentContext();

    // Use the same attribs you used in opengl_CreateContext
    int attribs[] = {
        0x2091, 4,
        0x2092, 3,
        0x9126, 0x00000002,
        0
    };

    auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    // The second parameter here IS the sharing parameter
    data->hglrc = wglCreateContextAttribsARB(data->hdc, main_hglrc, attribs);

    vp->RendererUserData = data;
}

void win32_ImGui_Renderer_DestroyWindow(ImGuiViewport* vp) {
    if (auto* data = (ViewportDataWin32*)vp->RendererUserData) {
        wglDeleteContext(data->hglrc);
        ReleaseDC((HWND)vp->PlatformHandle, data->hdc);
        delete data;
    }
    vp->RendererUserData = nullptr;
}

void win32_ImGui_Renderer_RenderWindow(ImGuiViewport* vp, void*) {
    if (auto* data = (ViewportDataWin32*)vp->RendererUserData) {
        BOOL switched = wglMakeCurrent(data->hdc, data->hglrc);
        if (!switched) {
            // If this prints, your handles are dead or invalid for this thread
            printf("IMGUI ERROR: Context switch failed. Error: %lu\n", GetLastError());
            return;
        }

        // If this clears the window but NO UI appears, your sharing failed
        glViewport(0, 0, (GLsizei)vp->Size.x, (GLsizei)vp->Size.y);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // Bright Pink
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(vp->DrawData);
    }
}

void win32_ImGui_Renderer_SwapBuffers(ImGuiViewport* vp, void*) {
    if (auto* data = (ViewportDataWin32*)vp->RendererUserData) {
        SwapBuffers(data->hdc);
    }
}

bool platform_imgui_init(Window* window) {
    auto* win32_window = reinterpret_cast<Win32_Window_Data*>(window->handle.data);
    if (!ImGui_ImplWin32_Init(win32_window->handle)) return false;

    // Register the "Glue" that allows the renderer to talk to Win32
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Renderer_CreateWindow = win32_ImGui_Renderer_CreateWindow;
    platform_io.Renderer_DestroyWindow = win32_ImGui_Renderer_DestroyWindow;
    platform_io.Renderer_RenderWindow = win32_ImGui_Renderer_RenderWindow;
    platform_io.Renderer_SwapBuffers = win32_ImGui_Renderer_SwapBuffers;

    return true;
}
