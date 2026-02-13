# Petrichor

This is a simple game engine designed to test my C++ development skills.

- The windowing APIs are management myself without SDL, GLFW or any other windowing library - though this might be an addition later due to the challenges of Windows.h / Win32 API
- Renderer code is mostly mine - using GLAD to load OpenGL functions for simplicity

- Future additions and plans include:
  - **Window / Compositors**
    - X11
    - Wayland
  - **Renderers**
    - Vulkan
    - DirectX3D 11
    - DirectX3D 12

This project is broken down into some main APIs:
- Graphics
  - This is the common handler for all the windowing and renderer code
- Platform
  - This is the specific implementation for each windowing API
  - Graphics is used as a unified interface into this code.
- Renderer
  - This is the specific implementation for each rendering API
  - Though each window has its own implementation for each renderer, meaning the code is split amongst here and the Platform
- Events
  - The primary event loop and handler
- App
  - The primary interface clients / consumers use the Petrichor Library