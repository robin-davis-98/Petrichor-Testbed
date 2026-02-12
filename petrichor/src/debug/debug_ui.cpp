#include "debug_ui.h"
#include <imgui.h>
#include "graphics/window.h"

void debug_RenderWindowSettings(Window* window) {
    ImGui::Begin("Engine Debugger");

    if (ImGui::CollapsingHeader("Window & Resolution", ImGuiTreeNodeFlags_DefaultOpen)) {
        WindowProperties& props = window->properties;

        ImGui::Text("Physical Size: %d x %d", props.width, props.height);
        ImGui::Text("Aspect Ratio %.2f", (float)props.width / (float)props.height);

        ImGui::Separator();

        ImGui::Text("Window Size (Target)");
        ImGui::InputInt("Width##windowed", (int*)&props.windowed_width);
        ImGui::InputInt("Height##windowed", (int*)&props.windowed_height);

        ImGui::Text("Internal Resolution (Logic)");
        ImGui::InputInt("Width##internal", (int*)&props.internal_resolution_width);
        ImGui::InputInt("Height##internal", (int*)&props.internal_resolution_height);

        if (ImGui::Button("Apply Resolution Changes")) {
            window_SetFlags(window, props.flags);
        }

        ImGui::Separator();

        bool is_fullscreen = (props.flags & WINDOW_FLAG_FULLSCREEN);
        bool is_borderless = (props.flags & WINDOW_FLAG_BORDERLESS);

        if (ImGui::Checkbox("Exclusive Fullscreen", &is_fullscreen)) {
            window_ToggleFullscreen(window);
        }

        if (ImGui::Checkbox("Borderless Fullscreen", &is_borderless)) {
            window_ToggleBorder(window);
        }
    }

    if (ImGui::CollapsingHeader("Renderer Statistics")) {
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    }

    ImGui::End();
}

void debug_RenderUI(Window *window, Renderer *renderer) {
    debug_RenderWindowSettings(window);
}

void debug_Init() {

}

void debug_Shutdown() {

}
