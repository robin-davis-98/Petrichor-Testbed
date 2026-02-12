target_sources(petrichor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
)

target_include_directories(petrichor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)