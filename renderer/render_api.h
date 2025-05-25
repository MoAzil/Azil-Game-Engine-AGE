
#pragma once

#include <iostream>
#include <string>
using namespace std;


extern int renderapi; // Declaration only

// Initializes shader and renderer state
void initRenderer();



// Renders a previously loaded mesh
void renderMesh();

// Deletes a previously loaded mesh
void deleteMesh();

// Cleans up all loaded data (meshes, shaders)
void cleanupRenderer();

