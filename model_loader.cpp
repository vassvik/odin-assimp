#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <stdio.h>
#include <vector>

struct Vec3f {
	float x, y, z;
};

struct Triangle {
	Vec3f v1; float _p1;
	Vec3f v2; float _p2;
	Vec3f v3; float _p3;
};

static void processMesh(aiMesh* mesh, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f> &vertices, std::vector<int> &indices);
static void processNode(aiNode* node, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f> &vertices, std::vector<int> &indices);

extern "C" Triangle *load_mesh(const char *filename, size_t *num_triangles);
extern "C" void free_mesh(Triangle *triangles);

void free_mesh(Triangle *triangles) {
	delete []triangles;
}

Triangle *load_mesh(const char *filename, size_t *num_triangles) {
	Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, 0);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {     // if is Not Zero
        printf("ERROR Assimp: %s\n", importer.GetErrorString());
        return nullptr;
    }

    std::vector<Vec3f> vertices;
	std::vector<int> indices;
    processNode(scene->mRootNode, aiMatrix4x4(), scene, vertices, indices);
    
	*num_triangles = indices.size() / 3;
    Triangle *triangles = new Triangle[*num_triangles];

    for (auto i = 0; i < *num_triangles; i++) {
    	triangles[i] = Triangle{
    		vertices[indices[3*i+0]], 0.0f, 
    		vertices[indices[3*i+1]], 0.0f,
    		vertices[indices[3*i+2]], 0.0f
    	};
    }

    return triangles;
}

void processNode(aiNode* node, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f> &vertices, std::vector<int> &indices) {
    auto currentTransformation = transformation * node->mTransformation;
    for (auto i = 0; i < node->mNumMeshes; i++) {
        processMesh(scene->mMeshes[node->mMeshes[i]], currentTransformation, scene, vertices, indices);
    }
    for (auto i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], currentTransformation, scene, vertices, indices);
    }
}

void processMesh(aiMesh* mesh, const aiMatrix4x4& transformation, const aiScene* scene, std::vector<Vec3f> &vertices, std::vector<int> &indices) {
    unsigned int lastNVertices = vertices.size();
    for (auto i = 0; i < mesh->mNumVertices; i++) {
        auto v = transformation * mesh->mVertices[i];
        vertices.push_back(Vec3f{v[0], v[1], v[2]});
    }
    
    for (auto i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
    	for (auto j = 0; j < face.mNumIndices-2; j++) {
    		indices.push_back(lastNVertices + face.mIndices[0]);
    		indices.push_back(lastNVertices + face.mIndices[j+1]);
    		indices.push_back(lastNVertices + face.mIndices[j+2]);
    	}
    }
}

/*

int main() {
	size_t num_triangles;
	Triangle *triangles = load_mesh("models/moon.FBX", &num_triangles);

	printf("%d triangles\n", (int)num_triangles);


	

    return 0;
}*/