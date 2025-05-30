#include <iostream>
#include <string>
#include <render_api.h>       // Your API abstraction header
#include <opengl_renderer.h>  // The OpenGL-specific implementation header
#include "mesh.h"             // Assumed to contain MeshData, MaterialData etc.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

int renderapi; // Global variable to select the rendering API

void initRenderer() {
    if (renderapi == 1) {
        glinitRenderer(); // Call the OpenGL-specific initialization
    } else {
        cout << "Renderer API not supported: " << renderapi << endl;
    }
}

void renderMesh(const string& meshID, const MeshData& mesh, const MaterialData& mat,
                 const float position[3], const float rotation[3]) {
    if (renderapi == 1) {
        glUploadMesh(meshID, mesh, mat, position, rotation); // Call the OpenGL-specific mesh upload
    } else {
        cout << "Renderer API not supported: " << renderapi << endl;
    }
}

void deleteMesh(const string& meshID) {
    if (renderapi == 1) {
        gldeleteMesh(meshID); // Call the OpenGL-specific mesh deletion
    } else {
        cout << "Renderer API not supported: " << renderapi << endl;
    }
}

void cleanupRenderer() {
    if (renderapi == 1) {
        glcleanupRenderer(); // Call the OpenGL-specific cleanup
    } else {
        cout << "Renderer API not supported: " << renderapi << endl;
    }
}

void RenderAllMeshes(unsigned int screenWidth, unsigned int screenHeight,
                     const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp){
    if (renderapi == 1) {
        // CORRECTED: Pass the variable names, not their types, to glRenderAllMeshes
        glRenderAllMeshes(screenWidth, screenHeight, cameraPos, cameraFront, cameraUp);
    } else {
        cout << "Renderer API not supported: " << renderapi << endl;
    }
}

// render_api.cpp (add this function)

// ... existing code ...

void setMeshTransform(const std::string& meshID, const glm::vec3& position,
                      const glm::vec3& rotationDegrees, const glm::vec3& scale) {
    if (renderapi == 1) {
        glSetMeshTransform(meshID, position, rotationDegrees, scale);
    } else {
        std::cerr << "Renderer API not supported for setMeshTransform: " << renderapi << std::endl;
    }
}