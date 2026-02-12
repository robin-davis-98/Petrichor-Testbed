cmake_minimum_required(VERSION 4.1)
project(gameA)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BACKEND "WIN32" CACHE STRING "Which backend to use")

set_property(CACHE BACKEND PROPERTY STRINGS "WIN32")


set(RUNTIME_NAME "game")
#set(WINDOW_TITLE "Window")

add_subdirectory(petrichor)
add_subdirectory(game)