#include "app/app.h"
#include "platform/platform.h"
#include <glad/glad.h>

#include <iostream>

App app_Create() {
    App app{};

    app.eventQueue = {};
    app.window = {};

    app.window.properties.windowed_width = 800;
    app.window.properties.windowed_height = 600;
    app.window.properties.flags = {};

    event_Init(&app.eventQueue);
    window_Create(&app.window, &app.eventQueue);
    renderer_Init(&app.renderer, RENDERER_BACKEND_OPENGL, &app.window);

    glGenBuffers(1, &app.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app.vbo);

    glGenVertexArrays(1, &app.vao);
    glBindVertexArray(app.vao);

    window_Show(&app.window);

    app.running = true;

    std::cout << "App Created" << std::endl;

    return app;
}

void app_ProcessEvents(App& app) {
    while (app.eventQueue.count > 0) {
        Event e = event_Pop(&app.eventQueue);

        switch (e.type) {
            case EVENT_WINDOW_CLOSE: {
                app.running = false;
            } break;
            case EVENT_WINDOW_SET_BORDERLESS: {
                window_ToggleBorder(&app.window);
            } break;
            case EVENT_WINDOW_SET_FULLSCREEN: {
                window_ToggleFullscreen(&app.window);
            } break;
            case EVENT_WINDOW_RESIZE: {
                auto* data = reinterpret_cast<WindowStatusData*>(e.data);
                renderer_Resize(&app.renderer, data->width, data->height);

                app.window.properties.width = data->width;
                app.window.properties.height = data->height;

                if (!(app.window.properties.flags & WINDOW_FLAG_FULLSCREEN)) {
                    app.window.properties.internal_resolution_width = data->width;
                    app.window.properties.internal_resolution_height = data->height;
                }
            }
            default: {}
        }
    }
}


void app_Update(App& app) {
    /*
     * Handles each frame
     */
    window_Update(&app.window, app.eventQueue);
    app_ProcessEvents(app);

    float red = 29.0f / 255.0f;
    float green = 37.0f / 255.0f;
    float blue = 45.0f / 255.0f;

    renderer_Clear(&app.renderer, red, green , blue, 1.0f);



    renderer_Present(&app.renderer);
}

void app_Run(App& app) {
    /*
     * Handles the update loop
     */
    std::cout << "Starting App Loop" << std::endl;

    while (app.running) {
        app_Update(app);
    }

}