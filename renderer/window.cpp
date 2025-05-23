#include <iostream>
#include "window.h"
#include "opengl/glad/glad.h"
#include "opengl/GLFW/glfw3.h"
#include <chrono>
#include <sstream>
#include <string>

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int hello()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "s", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // Declare timing variables OUTSIDE the loop
    auto lastTime = std::chrono::high_resolution_clock::now();
    int nbFrames = 0;

    // === Main loop ===
    while (!glfwWindowShouldClose(window))
    {
        // FPS counter
        auto currentTime = std::chrono::high_resolution_clock::now();
        nbFrames++;
        float elapsed = std::chrono::duration<float>(currentTime - lastTime).count();
        if (elapsed >= 1.0f)
        {
            std::ostringstream ss;
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

        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
