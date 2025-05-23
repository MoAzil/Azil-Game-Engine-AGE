cmake_minimum_required(VERSION 3.10)
project(Azil_Game_Engine VERSION 0.1.0 LANGUAGES C CXX)

# Define executable
add_executable(AGE main.cpp)

# Define the window/render library (you probably meant one library with these sources)
add_library(window STATIC
    renderer/window.cpp
    renderer/opengl/opengl_renderer.cpp
    renderer/opengl/glad.c
)

# Include directories
target_include_directories(window PUBLIC renderer/opengl)
target_include_directories(AGE PRIVATE renderer/opengl)

# Link GLFW and OpenGL
# NOTE: Adjust glfw3.lib path if you're on Windows and using a custom path
target_link_libraries(window
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/opengl/glfw3.lib
    opengl32
)

# Link everything to the main executable
target_link_libraries(AGE PRIVATE window)
