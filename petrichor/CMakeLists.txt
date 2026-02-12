cmake_minimum_required(VERSION 4.1)
project(petrichor)

set(CMAKE_CXX_STANDARD 20)

add_library(petrichor SHARED)

add_subdirectory(vendors)

target_include_directories(petrichor
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(petrichor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/app.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/events/event.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/renderer.cpp
)

target_compile_definitions(petrichor
        PRIVATE
            ENGINE_EXPORTS
)

if(WINDOW_TITLE)
    target_compile_definitions(petrichor
        PRIVATE
            APP_NAME="${WINDOW_TITLE}"
    )
endif()

if (BACKEND STREQUAL "WIN32")
    target_compile_definitions(petrichor PRIVATE BACKEND_WIN32)
    target_sources(petrichor PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32/window.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32/events.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32/gl.cpp
    )
    target_link_libraries(petrichor PRIVATE
            user32
            gdi32
            opengl32.lib
    )
endif()