cmake_minimum_required(VERSION 3.10.0)
project(Azil_Game_Engine VERSION 0.1.0 LANGUAGES C CXX)

# Add all source files here
add_executable(AGE main.cpp)

add_library(
    hello
    renderer/window.cpp
)
# Optional: include directories for headers

target_link_libraries(AGE PRIVATE hello)
target_include_directories(hello PUBLIC renderer/opengl)
add_library(glad STATIC renderer/opengl/glad.c)
target_include_directories(glad PUBLIC renderer/opengl/)
target_include_directories(AGE PRIVATE renderer/opengl)

target_link_libraries(AGE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/opengl/glfw3.lib
    opengl32
    glad
    hello
)


# Packaging (optional)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
