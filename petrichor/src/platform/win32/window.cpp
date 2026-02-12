#include <cassert>
#include <windows.h>
#include <iostream>

#include "graphics/window.h"
#include "platform/platform.h"
#include "platform/win32/win32_internal.h"

#include <imgui.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK win32_event_callback(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    if (ImGui_ImplWin32_WndProcHandler(handle, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
        case WM_CLOSE: {
            DestroyWindow(handle);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        case WM_SIZE: {
            uint32_t width = LOWORD(lParam);
            uint32_t height = HIWORD(lParam);

            if (auto* queue =(EventQueue*)GetWindowLongPtr(handle, GWLP_USERDATA)) {
                Event e = {EVENT_WINDOW_RESIZE};
                auto* data = reinterpret_cast<WindowStatusData*>(e.data);
                data->width = width;
                data->height = height;
                event_Push(queue, e);
            }
        } break;

        default: {
            result = DefWindowProcA(handle, msg, wParam, lParam);
        }
    }

    return result;
}

void win32_Create_Window(Win32_Window_Data* window_data, const WindowProperties& props, EventQueue* queue) {
    SetProcessDPIAware();
    *window_data = {};

    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "Petrichor Window";
    wc.lpfnWndProc = win32_event_callback;
    wc.hbrBackground = NULL;

    if (!RegisterClassA(&wc)) {
        return;
    }

    window_data->handle = CreateWindowExA(0,
        "Petrichor Window",
        APP_NAME,
        0,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0,
        NULL,
        NULL,
        instance,
        NULL
        );

    if (window_data->handle) {
        SetWindowLongPtrA(window_data->handle, GWLP_USERDATA, (LONG_PTR)queue);
    }
}

static DWORD win32_GetStyle(uint32_t flags) {
    DWORD style = 0;
    if (flags & (WINDOW_FLAG_BORDERLESS | WINDOW_FLAG_FULLSCREEN)) {
        style = WS_POPUP;
    } else {
        style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        if (flags & WINDOW_FLAG_RESIZABLE) {
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }
    }

    if (!(flags & WINDOW_FLAG_HIDDEN)) {
        style |= WS_VISIBLE;
    }

    return style;
}

void window_SetFlags(Window* window, uint32_t flags) {
    auto* internal = reinterpret_cast<Win32_Window_Data*>(window->handle.data);
    if (!internal->handle) return;

    DWORD dwStyle = win32_GetStyle(flags);

    DWORD dwEXStyle = WS_EX_APPWINDOW;
    if (!(flags & WINDOW_FLAG_FULLSCREEN)) {
        dwEXStyle |= WS_EX_WINDOWEDGE;
    }

    int32_t x, y, w, h = 0;
    HWND zOrder = HWND_TOP;

    if (flags & (WINDOW_FLAG_FULLSCREEN | WINDOW_FLAG_BORDERLESS )) {
        w = GetSystemMetrics(SM_CXSCREEN);
        h = GetSystemMetrics(SM_CYSCREEN);
        x = 0; y = 0;

        zOrder = (flags & WINDOW_FLAG_FULLSCREEN) ? HWND_TOPMOST : HWND_TOP;

    } else {
        w = window->properties.windowed_width;
        h = window->properties.windowed_height;

        RECT rect = {0, 0, w, h};
        AdjustWindowRectEx(&rect, dwStyle, FALSE, dwEXStyle);

        w = rect.right - rect.left;
        h = rect.bottom - rect.top;

        x = ((GetSystemMetrics(SM_CXSCREEN) - w) / 2);
        y = ((GetSystemMetrics(SM_CYSCREEN) - h) / 2);

        zOrder = HWND_NOTOPMOST;
    }

    SetWindowLongPtrA(internal->handle, GWL_STYLE, dwStyle);
    SetWindowLongPtrA(internal->handle, GWL_EXSTYLE, dwEXStyle);

    SetWindowPos(internal->handle, zOrder, x, y, w, h, SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_SHOWWINDOW );

    window->properties.flags = flags;

    if (flags & WINDOW_FLAG_HIDDEN) {
        ShowWindow(internal->handle, SW_HIDE);
    } else {
        ShowWindow(internal->handle, SW_SHOW);
        SetForegroundWindow(internal->handle);
        SetFocus(internal->handle);
    }
}

void window_Hide(Window* window) {
    window->properties.flags &= WINDOW_FLAG_HIDDEN;
    window_SetFlags(window, window->properties.flags);
}

void window_Show(Window* window) {
    window->properties.flags &= ~WINDOW_FLAG_HIDDEN;
    window_SetFlags(window, window->properties.flags);
}

void window_ToggleFullscreen(Window* window) {
    uint32_t flags = window->properties.flags;

    flags ^= WINDOW_FLAG_FULLSCREEN;

    if (flags & WINDOW_FLAG_FULLSCREEN) {
        flags |= WINDOW_FLAG_BORDERLESS;
    } else {
        flags &= ~WINDOW_FLAG_BORDERLESS;

        window->properties.width = window->properties.internal_resolution_width;
        window->properties.height = window->properties.internal_resolution_height;
    }

    flags &= ~WINDOW_FLAG_HIDDEN;

    window_SetFlags(window, flags);
}

void window_ToggleBorder(Window* window) {
    uint32_t flags = window->properties.flags;

    if (!(flags & WINDOW_FLAG_FULLSCREEN)) {
        flags ^= WINDOW_FLAG_BORDERLESS;
        flags &= ~WINDOW_FLAG_HIDDEN;

        window_SetFlags(window, flags);
    }
}

void window_Create(Window* window, EventQueue* queue) {
    assert(sizeof(Win32_Window_Data) <= sizeof(Window_Handle::data));
    auto* internal = reinterpret_cast<Win32_Window_Data*>(window->handle.data);

    win32_Create_Window(internal, window->properties, queue);
    window_SetFlags(window, window->properties.flags);
}

void window_Update(Window* window, EventQueue& queue) {
    auto* internal = reinterpret_cast<Win32_Window_Data*>(window->handle.data);
    MSG msg = {};

    bool toggle_requested = false;

    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            event_Push(&queue, { EVENT_WINDOW_CLOSE });
        }

        if (msg.message == WM_KEYDOWN) {
            bool is_repeat = (msg.lParam & (1 << 30)) != 0;

            Event e = {};
            switch (msg.wParam) {
                case VK_ESCAPE: {
                    e.type = EVENT_WINDOW_CLOSE;
                } break;
                case VK_F9: {
                    if (!is_repeat) {
                        e.type = EVENT_WINDOW_SET_BORDERLESS;
                        auto* data = reinterpret_cast<WindowStatusData*>(e.data);
                        data->border = !(window->properties.flags & WINDOW_FLAG_BORDERLESS);
                    }
                } break;
                case VK_F11: {
                    if (!is_repeat) {
                        e.type = EVENT_WINDOW_SET_FULLSCREEN;
                        auto* data = reinterpret_cast<WindowStatusData*>(e.data);
                        data->fullscreen = !(window->properties.flags & WINDOW_FLAG_FULLSCREEN);
                    }
                } break;
                default : {}
            }

            if (e.type != 0) {
                event_Push(&queue, e);
            }
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void* window_GetNativeHandle(const Window* window) {
    auto* internal = reinterpret_cast<const Win32_Window_Data*>(window->handle.data);
    return (void*)internal->handle;
}