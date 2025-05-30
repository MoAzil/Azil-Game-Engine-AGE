#ifndef MESH_IMPORTER_H
#define MESH_IMPORTER_H

#include <string>

// Forward declarations if needed (avoid including too many headers here)
struct MeshData;
struct MaterialData;

// Main import function
void ImportAndSendMesh(const std::string& path, const std::string& meshID,
                       const float position[3], const float rotationEuler[3]);

#endif // MESH_IMPORTER_H
