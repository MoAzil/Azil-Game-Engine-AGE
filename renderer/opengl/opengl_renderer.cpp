#include "opengl_renderer.h"
#include "glad/glad.h"
#include "mesh.h" // Contains Vertex, MeshData, MaterialData
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Helper for shader error checking
void checkShaderCompileErrors(GLuint shader, std::string type);

struct MeshGPU {
    GLuint vao, vbo, ebo;
    size_t indexCount;
    glm::vec3 position;
    glm::vec3 rotation; // Stores rotation in degrees
    glm::vec3 scale;    // --- NEW: Add scale member ---
    glm::vec3 color;
};

static std::unordered_map<std::string, MeshGPU> meshes;
static GLuint shaderProgram = 0;

// (Your vertexShaderSource and fragmentShaderSource remain the same)
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightDir;
uniform vec3 viewPos;

void main() {
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * color;

    float diff = max(dot(normalize(Normal), -normalize(lightDir)), 0.0);
    vec3 diffuse = diff * color;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
)";


void glRotateAllMeshes(float xDegreesPerFrame, float yDegreesPerFrame, float zDegreesPerFrame) {
    for (auto& [id, mesh] : meshes) {
        mesh.rotation.x += xDegreesPerFrame;
        mesh.rotation.y += yDegreesPerFrame;
        mesh.rotation.z += zDegreesPerFrame;
    }
}


void glinitRenderer() {
    glDisable(GL_CULL_FACE); // Consider enabling GL_DEPTH_TEST and GL_CULL_FACE later

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void glUploadMesh(const std::string& meshID, const MeshData& mesh, const MaterialData& mat,
                  const float pos[3], const float rot[3]) {
    auto it = meshes.find(meshID);
    if (it == meshes.end()) { // Mesh does not exist, create new
        MeshGPU gpu{};
        gpu.position = glm::vec3(pos[0], pos[1], pos[2]);
        gpu.rotation = glm::vec3(rot[0], rot[1], rot[2]);
        gpu.scale = glm::vec3(1.0f); // --- Initialize scale to 1.0f for new meshes ---
        gpu.color = glm::vec3(mat.diffuseColor[0], mat.diffuseColor[1], mat.diffuseColor[2]);
        gpu.indexCount = mesh.indices.size();

        glGenVertexArrays(1, &gpu.vao);
        glGenBuffers(1, &gpu.vbo);
        glGenBuffers(1, &gpu.ebo);

        glBindVertexArray(gpu.vao);

        glBindBuffer(GL_ARRAY_BUFFER, gpu.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texcoord attribute (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO

        meshes[meshID] = gpu;
    } else { // Mesh exists, update its transformation and color
        MeshGPU& gpu = it->second;
        gpu.position = glm::vec3(pos[0], pos[1], pos[2]);
        gpu.rotation = glm::vec3(rot[0], rot[1], rot[2]);
        gpu.color = glm::vec3(mat.diffuseColor[0], mat.diffuseColor[1], mat.diffuseColor[2]);
        // Note: Scale is NOT updated here. Use glSetMeshTransform for scale.
    }
}

// --- NEW FUNCTION IMPLEMENTATION ---
void glSetMeshTransform(const std::string& meshID, const glm::vec3& position,
                        const glm::vec3& rotationDegrees, const glm::vec3& scale) {
    auto it = meshes.find(meshID);
    if (it != meshes.end()) {
        MeshGPU& gpu = it->second;
        gpu.position = position;
        gpu.rotation = rotationDegrees;
        gpu.scale = scale;
    } else {
        std::cerr << "Warning: Attempted to set transform for non-existent mesh with ID: " << meshID << std::endl;
    }
}
// --- END NEW FUNCTION IMPLEMENTATION ---


void glRenderAllMeshes(unsigned int screenWidth, unsigned int screenHeight,
                       const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp) {
    glUseProgram(shaderProgram);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    GLint lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    // Camera/View matrix
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Projection matrix (Perspective)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Light direction (points from light source TO the scene/objects)
    glm::vec3 lightDirection = glm::normalize(glm::vec3(0.5f, -1.0f, 0.3f));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirection));

    // Camera position for potential future specular lighting or other effects
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));


    // Animate rotation for one of the objects (e.g., cube)
    // Removed the automatic rotation here to give full control via glSetMeshTransform
    /*
    if (meshes.count("myCube")) {
        meshes["myCube"].rotation.y += 0.5f; // Rotate cube around Y axis
    }
    */


    for (const auto& [id, mesh] : meshes) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, mesh.position);
        model = glm::rotate(model, glm::radians(mesh.rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(mesh.rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(mesh.rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, mesh.scale); // --- NEW: Apply scale to the model matrix ---

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(colorLoc, 1, glm::value_ptr(mesh.color));

        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0); // Unbind VAO
    glUseProgram(0);     // Unuse shader program
}


void gldeleteMesh(const std::string& meshID) {
    auto it = meshes.find(meshID);
    if (it != meshes.end()) {
        glDeleteBuffers(1, &it->second.vbo);
        glDeleteBuffers(1, &it->second.ebo);
        glDeleteVertexArrays(1, &it->second.vao);
        meshes.erase(it);
    }
}

void glcleanupRenderer() {
    for (auto& [id, mesh] : meshes) {
        glDeleteBuffers(1, &mesh.vbo);
        glDeleteBuffers(1, &mesh.ebo);
        glDeleteVertexArrays(1, &mesh.vao);
    }
    meshes.clear();

    glDeleteProgram(shaderProgram);
}

// Helper function to check for shader compilation/linking errors
void checkShaderCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}