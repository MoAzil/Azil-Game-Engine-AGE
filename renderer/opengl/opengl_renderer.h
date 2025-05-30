#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <string>
#include <glm/glm.hpp> // Include GLM here if you use its types in the header

// Forward declarations to avoid circular includes
struct MeshData;
struct MaterialData;

void glinitRenderer();
void glUploadMesh(const std::string& meshID, const MeshData& mesh, const MaterialData& mat,
                  const float pos[3], const float rot[3]);
void glRenderAllMeshes(unsigned int screenWidth, unsigned int screenHeight,
                       const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp);
void gldeleteMesh(const std::string& meshID);
void glcleanupRenderer();
void glRotateAllMeshes(float xDegreesPerFrame, float yDegreesPerFrame, float zDegreesPerFrame);

// --- NEW FUNCTION DECLARATION ---
void glSetMeshTransform(const std::string& meshID, const glm::vec3& position,
                        const glm::vec3& rotationDegrees, const glm::vec3& scale);
// --- END NEW FUNCTION DECLARATION ---

#endif // OPENGL_RENDERER_H