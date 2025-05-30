#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render_logic.h" // Your header for render_this, prepaire_renderer, stop_renderer
#include "render_api.h"   // Assuming this header declares initRenderer, cleanupRenderer, RenderAllMeshes
#include <fstream>
#include <sstream>

// Assuming 'renderapi' is declared elsewhere and globally accessible,
// or defined in render_api.h
// e.g., extern int renderapi;

void prepaire_renderer() {
    // If renderapi is meant to select a specific rendering backend (like OpenGL vs DirectX)
    // this setup needs to happen before initRenderer is called.
    // If renderapi is a global variable from render_api.h, this is fine.
    // Otherwise, ensure it's declared somewhere appropriate.
    // For now, assuming renderapi is a global or static variable you control.
    renderapi = 1; // Sets the active rendering API to 1 (e.g., OpenGL)
    initRenderer(); // Calls the actual initialization function from render_api.h/cpp
}

void stop_renderer() {
    cleanupRenderer(); // Calls the actual cleanup function from render_api.h/cpp
}

// This function now correctly forwards the parameters it receives
void render_this(unsigned int screenWidth, unsigned int screenHeight,
                 const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp) {
    // CORRECTED: Pass the variable names, not their types.
    RenderAllMeshes(screenWidth, screenHeight, cameraPos, cameraFront, cameraUp);
}