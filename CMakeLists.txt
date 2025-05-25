cmake_minimum_required(VERSION 3.10)
project(Azil_Game_Engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define executable
add_executable(AGE main.cpp)

# ImGui static library
add_library(imgui STATIC
    engine_ui/imgui/imgui.cpp
    engine_ui/imgui/imgui_draw.cpp
    engine_ui/imgui/imgui_tables.cpp
    engine_ui/imgui/imgui_widgets.cpp
    engine_ui/imgui/imgui_impl_opengl3.cpp
    engine_ui/imgui/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine_ui/imgui)
target_link_libraries(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/renderer/opengl/glfw3.lib)
# Window/rendering static library
add_library(window_manager STATIC
    renderer/window.cpp
    renderer/render_logic.cpp
    renderer/render_api.cpp
    renderer/opengl/opengl_renderer.cpp
    renderer/opengl/glad.c
)
target_include_directories(imgui PUBLIC
    renderer/GLFW
    renderer

)
target_include_directories(window_manager PUBLIC
    renderer/opengl
    maths/glm
    maths/glm/gtc
    engine_ui
)

target_link_libraries(window_manager PRIVATE
    imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/opengl/glfw3.lib
    opengl32
)


# Link libraries to the executable
target_link_libraries(AGE PRIVATE window_manager)
