add_subdirectory(glad)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

if (BACKEND STREQUAL "WIN32")
    target_sources(imgui PRIVATE
            ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    )

    target_link_libraries(imgui PUBLIC
            shell32   # ImGui often needs this for opening URLs/folders
            dwmapi    # Required for ImGui Docking/Viewports on Windows
            imm32     # Required for ImGui Input Method Editor (IME)
    )
endif()