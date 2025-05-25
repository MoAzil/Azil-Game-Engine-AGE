#include <iostream>

#include "render_logic.h"
#include "render_api.h"
#include <fstream>
#include <sstream>

void prepaire_renderer() {
    renderapi = 1;
    initRenderer();
}

void stop_renderer() {
    cleanupRenderer();
};

void launch_renderer() {
    renderMesh();
    renderapi = 1;
    deleteMesh();

};