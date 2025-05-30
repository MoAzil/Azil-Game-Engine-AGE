#pragma once
#include <string>
#include <vector>

struct Vertex {
    float position[3];
    float normal[3];
    float texcoord[2];
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

struct MaterialData {
    std::string diffuseTexturePath;
    float diffuseColor[3] = {1.0f, 1.0f, 1.0f};
    float shininess = 32.0f;
};
