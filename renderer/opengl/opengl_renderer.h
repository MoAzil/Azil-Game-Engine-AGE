
#define OPENGL_RENDERER_H

#include <string>

// Initializes shader and renderer state
void glinitRenderer();


// Renders a previously loaded mesh
void glrenderMesh();

// Deletes a previously loaded mesh
void gldeleteMesh();

// Cleans up all loaded data (meshes, shaders)
void glcleanupRenderer();

