cmake_minimum_required(VERSION 3.20)
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

install(TARGETS game
        RUNTIME DESTINATION bin)

install(TARGETS petrichor
        RUNTIME DESTINATION .
        LIBRARY DESTINATION .
        ARCHIVE DESTINATION lib
)

if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
    install(FILES "${CMAKE_SOURCE_DIR}/README.md" DESTINATION .)
endif()

if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/game/assets")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/game/assets" DESTINATION .)
endif()

set(CPACK_PACKAGE_NAME "Petrichor")
set(CPACK_PACKAGE_VENDOR "Robin Davis")
set(CPACK_PACKAGE_VERSION "0.0.1")

set(CPACK_GENERATOR "ZIP;7Z")

include(CPack)