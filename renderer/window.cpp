#include <iostream>
using namespace std;
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <mesh_importer.h> // Assuming this exists and has ImportAndSendMesh
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Required for glm::value_ptr
#include "window.h" // Assuming this defines window related constants/functions (if not, consider removing or adding constants here)
#include "opengl/glad/glad.h" // Make sure glad is correctly located relative to this file
#include <GLFW/glfw3.h>
#include <chrono>
#include <sstream>
#include <string>
#include "render_logic.h" // Assumed to contain prepaire_renderer, render_this, stop_renderer
#include "render_api.h"   // Required to use setMeshTransform

// Window dimensions (good practice to define as constants)
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera Variables (remain global or static)
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// Global variables for ImGui input and mesh control
// Moved meshFilePath here from previous global scope.
std::string meshFilePath = "test/untitled.fbx"; // Default value for file path
char meshIDBuffer[128] = "myCube"; // Buffer for ImGui InputText, default ID
std::string currentMeshID; // To store the actual string ID from the buffer

glm::vec3 meshPos = glm::vec3(0.0f); // Default position for the controlled mesh
glm::vec3 meshRot = glm::vec3(0.0f); // Default rotation (degrees) for the controlled mesh
glm::vec3 meshScale = glm::vec3(1.0f); // Default scale for the controlled mesh


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


int window_manager()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    prepaire_renderer(); // Initializes your renderer

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    auto lastTime = chrono::high_resolution_clock::now();
    int nbFrames = 0;

    // Initialize currentMeshID with the default buffer value
    currentMeshID = meshIDBuffer;

    while (!glfwWindowShouldClose(window))
    {
        auto currentTime = chrono::high_resolution_clock::now();
        float deltaTime = chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        nbFrames++;
        if (deltaTime >= 1.0f)
        {
            ostringstream ss;
            ss << "OpenGL Window - " << nbFrames << " FPS";
            glfwSetWindowTitle(window, ss.str().c_str());
            nbFrames = 0;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- ImGui UI ---
        ImGui::Begin("Scene Control");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        // Fixed ImGui::InputText usage: needs char* and buffer size
        ImGui::InputText("Mesh Path", meshFilePath.data(), meshFilePath.capacity() + 1); // Using .data() for string buffer
        if (ImGui::IsItemDeactivatedAfterEdit()) { // Resize string if user typed more
            meshFilePath.resize(strlen(meshFilePath.data()));
        }

        ImGui::InputText("Mesh ID to Load", meshIDBuffer, sizeof(meshIDBuffer)); // Direct char buffer for ImGui
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            currentMeshID = meshIDBuffer; // Update std::string only after editing stops
        }

        if (ImGui::Button("Load Mesh")) {
            // Convert glm::vec3 to float[3] for ImportAndSendMesh initial load
            float tempPos[3] = {meshPos.x, meshPos.y, meshPos.z};
            float tempRot[3] = {meshRot.x, meshRot.y, meshRot.z};
            // Now, ImportAndSendMesh uses the ID you typed in the ImGui field
            // and the initial position/rotation from your glm::vec3 variables
            ImportAndSendMesh(meshFilePath, currentMeshID, tempPos, tempRot);
        }

        // --- ImGui sliders controlling mesh transform via glm::vec3s ---
        ImGui::Separator();
        ImGui::Text("Controlled Mesh: %s", currentMeshID.c_str());
        ImGui::SliderFloat3("Position", glm::value_ptr(meshPos), -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotation (Deg)", glm::value_ptr(meshRot), 0.0f, 360.0f);
        ImGui::SliderFloat3("Scale", glm::value_ptr(meshScale), 0.1f, 5.0f); // Prevent 0 scale

        // --- Call setMeshTransform every frame to update the mesh's properties ---
        setMeshTransform(currentMeshID, meshPos, meshRot, meshScale);

        ImGui::Separator();
        ImGui::SliderFloat3("Camera Position", glm::value_ptr(cameraPos), -10.0f, 10.0f);
        ImGui::Text("Camera Front: (%.2f, %.2f, %.2f)", cameraFront.x, cameraFront.y, cameraFront.z);
        ImGui::Text("Camera Up: (%.2f, %.2f, %.2f)", cameraUp.x, cameraUp.y, cameraUp.z);


        ImGui::End();
        // --- End ImGui UI ---

        // Input
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_this(SCR_WIDTH, SCR_HEIGHT, cameraPos, cameraFront, cameraUp);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    stop_renderer();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}