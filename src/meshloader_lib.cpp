#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <vector>

struct Vec3f
{
    float x, y, z;
};

template <typename T>
struct Slice {
    T* data;
    size_t len;

    Slice() {
        data = nullptr;
        len = 0;
    }

    Slice(const T* in_data, size_t in_len) {
        if (in_data && in_len > 0) {
            len = in_len;
            data = new T[in_len];
            memcpy(data, in_data, sizeof(T) * len);
        } else {
            len = 0;
            data = nullptr;
        }
    }
};

struct MeshData {
    Slice<Vec3f> vertices;
    Slice<int>   indices;
};

static void processMesh(aiMesh* mesh, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f>& vertices, std::vector<int>& indices);
static void processNode(aiNode* node, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f>& vertices, std::vector<int>& indices);

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void processNode(aiNode* node, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f>& vertices, std::vector<int>& indices) {
    auto currentTransformation = transformation * node->mTransformation;
    for (auto i = 0; i < node->mNumMeshes; i++) {
        processMesh(scene->mMeshes[node->mMeshes[i]], currentTransformation, scene, vertices, indices);
    }
    for (auto i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], currentTransformation, scene, vertices, indices);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void processMesh(aiMesh* mesh, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f>& vertices, std::vector<int>& indices) {
    unsigned int lastNVertices = vertices.size();
    for (auto i = 0; i < mesh->mNumVertices; i++) {
        auto v = transformation * mesh->mVertices[i];
        vertices.push_back(Vec3f{ v[0], v[1], v[2] });
    }

    for (auto i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (auto j = 0; j < face.mNumIndices - 2; j++) {
            indices.push_back(lastNVertices + face.mIndices[0]);
            indices.push_back(lastNVertices + face.mIndices[j + 1]);
            indices.push_back(lastNVertices + face.mIndices[j + 2]);
        }
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
extern "C" MeshData load_mesh_data(const char* filename);
extern "C" void     free_mesh_data(MeshData* mesh_data);

void free_mesh_data(MeshData* mesh_data) {
    if (!mesh_data) return;

    if (mesh_data->vertices.data) {
        delete[] mesh_data->vertices.data;
    }

    if (mesh_data->indices.data) {
        delete[] mesh_data->indices.data;
    }

    mesh_data->vertices.data = nullptr;
    mesh_data->vertices.len = 0;
    
    mesh_data->indices.data = nullptr;
    mesh_data->indices.len = 0;
}

MeshData load_mesh_data(const char* filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, 0);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("ERROR Assimp: %s\n", importer.GetErrorString());
        MeshData data;
        return data;
    }

    std::vector<Vec3f> vertices;
    std::vector<int>   indices;
    processNode(scene->mRootNode, aiMatrix4x4(), scene, vertices, indices);

    return {
        {vertices.data(), vertices.size()},
        {indices.data(), indices.size()}
    };
}
