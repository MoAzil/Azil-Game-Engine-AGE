// render_api.h
#pragma once
#include <string>
#include <vector>
#include "mesh.h"
#include <glm/glm.hpp> // Required for glm::vec3

extern int renderapi;

void initRenderer();
void RenderAllMeshes(unsigned int screenWidth, unsigned int screenHeight,
                     const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp);
void renderMesh(const std::string& meshID, const MeshData& mesh, const MaterialData& mat,
                const float position[3], const float rotation[3]);
void cleanupRenderer();
void deleteMesh(const std::string& meshID);

// NEW: Add this declaration
void setMeshTransform(const std::string& meshID, const glm::vec3& position,
                      const glm::vec3& rotationDegrees, const glm::vec3& scale);