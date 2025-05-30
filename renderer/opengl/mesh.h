#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

// Simple vertex with only position for now
#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

// Mesh data containing vertices and indices
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

// Material data containing basic color info
struct MaterialData {
    float diffuseColor[3]; // RGB values between 0 and 1
};

#endif // MESH_H
