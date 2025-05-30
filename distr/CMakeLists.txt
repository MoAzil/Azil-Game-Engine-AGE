# Create flecs static library
add_library(flecs STATIC flecs.c)

# Expose the current folder as an include path
target_include_directories(flecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
