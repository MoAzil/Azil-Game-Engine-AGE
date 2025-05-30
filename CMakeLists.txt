cmake_minimum_required(VERSION 3.10)
project(Azil_Game_Engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add external dependencies
add_subdirectory(distr)

# ========================
# Common Libraries (Global)
# ========================
add_library(common_libs INTERFACE)

target_include_directories(common_libs INTERFACE
    renderer
    renderer/opengl
    maths/
    engine_ui
    engine_ui/imgui
    resources    
    resources/assimp    # <-- add this line here
)


target_link_libraries(common_libs INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/opengl/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/assimp-vc143-mtd.lib
    opengl32
)

# ========================
# ImGui Static Library
# ========================
add_library(imgui STATIC
    engine_ui/imgui/imgui.cpp
    engine_ui/imgui/imgui_draw.cpp
    engine_ui/imgui/imgui_tables.cpp
    engine_ui/imgui/imgui_widgets.cpp
    engine_ui/imgui/imgui_impl_opengl3.cpp
    engine_ui/imgui/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC engine_ui/imgui)
target_link_libraries(imgui PRIVATE common_libs)

# ========================
# Window Manager Module
# ========================
add_library(window_manager STATIC
    renderer/window.cpp
    renderer/render_logic.cpp
    renderer/render_api.cpp
    renderer/opengl/opengl_renderer.cpp
    renderer/opengl/glad.c
    resources/mesh_importer.cpp
)

target_link_libraries(window_manager PRIVATE imgui common_libs)

# ========================
# Project Maker Module
# ========================
add_library(project_maker STATIC
    project_maker.cpp
)

target_link_libraries(project_maker PRIVATE common_libs imgui)

# ========================
# Main Executable
# ========================
add_executable(AGE main.cpp)

target_link_libraries(AGE
    PRIVATE window_manager
    PRIVATE project_maker
    PRIVATE flecs
    PRIVATE common_libs
)
