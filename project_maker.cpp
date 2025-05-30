#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <filesystem>
#include <fstream>
#include <iostream>


using json = nlohmann::json;

int project_created;

void LoadProject(){
    std::cout<<"loaded"<<std::endl;
}




// New function that handles creating the project on disk
void CreateProject(const char* projectName, const char* projectPath) {
    std::string basePath = std::string(projectPath) + "/" + projectName;

    // Create required directories
    std::filesystem::create_directories(basePath + "/Scripts");
    std::filesystem::create_directories(basePath + "/Assets");
    std::filesystem::create_directories(basePath + "/Build");
    std::filesystem::create_directories(basePath + "/Extensions/Windows");
    std::filesystem::create_directories(basePath + "/Extensions/Linux");
    std::filesystem::create_directories(basePath + "/Extensions/Mac");
    std::filesystem::create_directories(basePath + "/Extensions/Android");

    // Create project config
    json project_json = {
        {"name", projectName},
        {"mainScene", "Main.scene"}
    };

    std::ofstream project_file(basePath + "/Project.age");
    project_file << project_json.dump(4);
    project_file.close();

    // Create empty template scene
    json scene_json = {
        {"sceneName", "Main"},
        {"entities", json::array()},  // will contain entities + children + components
        {"comments", "This is a template scene file. Entities will be added here during editing."},
        {"version", 1}
    };

    std::ofstream scene_file(basePath + "/Main.scene");
    scene_file << scene_json.dump(4);
    scene_file.close();

    std::cout << "Project created at: " << basePath << std::endl;
    project_created = 1;
}


// UI function only draws UI and gathers input
void CreateNewProjectUI() {
    static char projectName[128] = "";
    static char projectPath[256] = "";

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 viewportSize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(viewportSize);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Azil Game Project Maker Fullscreen", nullptr, window_flags);

    ImGui::InputText("Project Name", projectName, IM_ARRAYSIZE(projectName));
    ImGui::InputText("Project Path", projectPath, IM_ARRAYSIZE(projectPath));

    if (ImGui::Button("Create Project")) {
        CreateProject(projectName, projectPath);
    }
    if (ImGui::Button("load Project")) {
        LoadProject();
    }
    ImGui::End();
}

int project_window() {
    // Init GLFW
    if (!glfwInit()) return -1;

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Azil Project Maker", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130"); // Use appropriate GL version

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        CreateNewProjectUI();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        if (project_created == 1){
            break;
        };
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
