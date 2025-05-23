#include <iostream>
using namespace std;
#include "window.h"
#include "opengl/glad/glad.h"
#include "opengl/GLFW/glfw3.h"
#include <chrono>
#include <sstream>
#include <string>
#include "opengl/opengl_renderer.h"


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int window()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "s", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // Declare timing variables OUTSIDE the loop
    auto lastTime = chrono::high_resolution_clock::now();
    int nbFrames = 0;
    
    initRenderer();
    // === Main loop ===
    while (!glfwWindowShouldClose(window))
    {
        // FPS counter
        
        


        auto currentTime = chrono::high_resolution_clock::now();
        nbFrames++;
        float elapsed = chrono::duration<float>(currentTime - lastTime).count();
        if (elapsed >= 1.0f)
        {
            ostringstream ss;
            ss << "s - " << nbFrames << " FPS";
            glfwSetWindowTitle(window, ss.str().c_str());
            nbFrames = 0;
            lastTime = currentTime;
        }
        
        // input
        processInput(window);
        
        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderTriangle();
        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    cleanupRenderer();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
