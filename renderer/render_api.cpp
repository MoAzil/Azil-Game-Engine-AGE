#include <iostream>
#include <string>
#include "render_api.h"
#include "opengl/opengl_renderer.h"

using namespace std;

int renderapi;


void initRenderer() {
    if (renderapi == 1) {
        glinitRenderer();  // ← now actually calls the function
    } else {
        cout << "Renderer API not supported: " << renderapi  << endl;
    }
}



void renderMesh() {
    if (renderapi  == 1) {
        glrenderMesh();
    } else {
        cout << "Renderer API not supported: " << renderapi  << endl;
    }
}

void deleteMesh() {
    if (renderapi  == 1) {
        gldeleteMesh();
    } else {
        cout << "Renderer API not supported: " << renderapi  << endl;
    }
}

void cleanupRenderer() {
    if (renderapi  == 1) {
        glcleanupRenderer();
    } else {
        cout << "Renderer API not supported: " << renderapi  << endl;
    }
}
