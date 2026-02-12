#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct Win32_Window_Data {
    HWND handle;
    HINSTANCE instance;
    HDC deviceContext;
};

LRESULT CALLBACK win32_event_callback(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);