#include "renderer_opengl.h"
#include "renderer/opengl/gl_internal.h"
#include "graphics/window.h"
#include "graphics/renderer.h"
#include "platform/imgui.h"

#include "glad/glad.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>

bool opengl_backend_init(Renderer* renderer, void* window_handle) {
    auto* context = reinterpret_cast<GL_Renderer_Context*>(renderer->internal_state);
    Window* window = static_cast<Window*>(window_handle);

    if (!opengl_CreateContext(window, context)) return false;
    opengl_MakeCurrent(context);

    if (!gladLoadGLLoader((GLADloadproc)opengl_GetProcAddress)) return false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplOpenGL3_Init("#version 430");

    if (!platform_imgui_init(window)) return false;

    // NOW register the custom renderer hooks and tell ImGui we handle them
    io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

    return true;
}

void opengl_backend_begin_frame(Renderer* renderer) {
    ImGui_ImplOpenGL3_NewFrame();
    platform_imgui_new_frame();
    ImGui::NewFrame();
}

void opengl_backend_clear(Renderer* renderer, float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void opengl_backend_end_frame(Renderer* renderer) {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        opengl_PlatformRenderViewports();

        // This restores the context to our main window
        auto* context = reinterpret_cast<GL_Renderer_Context*>(renderer->internal_state);
        opengl_MakeCurrent(context);
    }
}

void opengl_backend_present(Renderer* renderer) {
    auto* context = reinterpret_cast<GL_Renderer_Context*>(renderer->internal_state);
    opengl_SwapBuffers(context);
}

void opengl_backend_resize(Renderer* renderer, uint32_t width, uint32_t height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}