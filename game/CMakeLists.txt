cmake_minimum_required(VERSION 4.1)
project(${RUNTIME_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(${RUNTIME_NAME}
        src/main.cpp
)

target_link_libraries(${RUNTIME_NAME}
    PRIVATE
        petrichor
)

target_include_directories(${RUNTIME_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/engine/include
)

add_dependencies(${RUNTIME_NAME} petrichor)
