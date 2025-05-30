#include <render_api.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include <string>
#include <mesh.h>


void ImportAndSendMesh(const std::string& path, const std::string& meshID,
                       const float position[3], const float rotationEuler[3]) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Failed to load mesh: " << importer.GetErrorString() << std::endl;
        return;
    }

    aiMesh* mesh = scene->mMeshes[0];

    MeshData meshData;
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex v{};
        v.position[0] = mesh->mVertices[i].x;
        v.position[1] = mesh->mVertices[i].y;
        v.position[2] = mesh->mVertices[i].z;

        v.normal[0] = mesh->mNormals[i].x;
        v.normal[1] = mesh->mNormals[i].y;
        v.normal[2] = mesh->mNormals[i].z;

        if (mesh->HasTextureCoords(0)) {
            v.texcoord[0] = mesh->mTextureCoords[0][i].x;
            v.texcoord[1] = mesh->mTextureCoords[0][i].y;
        }

        meshData.vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            meshData.indices.push_back(face.mIndices[j]);
        }
    }

    MaterialData mat{};
    if (mesh->mMaterialIndex < scene->mNumMaterials) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(1.0f, 1.0f, 1.0f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        mat.diffuseColor[0] = color.r;
        mat.diffuseColor[1] = color.g;
        mat.diffuseColor[2] = color.b;
    }

    renderMesh(meshID, meshData, mat, position, rotationEuler);
}
