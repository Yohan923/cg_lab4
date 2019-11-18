#include "Resources.h"
#include <stdafx.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include <MeshRenderer.h>
#include <MeshFilter.h>
#include <GameObject.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <SkyBoxRenderer.h>

using namespace Assimp;
using namespace std;

Resources::Resources()
{
}

Resources::~Resources()
{
}

GameObject* Resources::loadGameObject(const char* path, const char* vsPath, const char* fsPath)
{
	vector<Mesh*> meshes = loadMeshes(path);

	auto* gameObject = new GameObject();

	for (Mesh* mesh : meshes) {
		auto* meshFilter = new MeshFilter();
		meshFilter->mesh = mesh;
		gameObject->addComponent(meshFilter);
	}

	auto* renderer = new MeshRenderer(vsPath, fsPath);
	gameObject->addComponent(renderer);

	return gameObject;
}

GameObject* Resources::loadSkyBox()
{
	auto* gameObject = new GameObject();
	auto* mesh = new Mesh();

	vector<Mesh*> meshes = loadMeshes("resources/models/Cube.dae");

	for (Mesh* mesh : meshes) {
		auto* meshFilter = new MeshFilter();
		meshFilter->mesh = mesh;
		gameObject->addComponent(meshFilter);
	}

	auto* renderer = new SkyBoxRenderer();
	gameObject->addComponent(renderer);

	return gameObject;
}

vector<Mesh*> Resources::loadMeshes(const char* path)
{
	Importer importer;

	auto* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!scene) {
		fprintf(stderr, "Error: '%s'\n", importer.GetErrorString());
		throw 1;
	}

	vector<Mesh*> meshes;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		auto* aimesh = scene->mMeshes[i];

		assert(aimesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);
		
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		auto* vertices = new glm::vec3[aimesh->mNumVertices];
		auto* normals = new glm::vec3[aimesh->mNumVertices];
		auto* triangles = new glm::uvec3[aimesh->mNumVertices];
		auto* textures = new glm::vec2[aimesh->mNumVertices];

		for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
		{
			auto vertex = toGlmVec3(aimesh->mVertices[i]);
			vertices[i] = vertex;

			auto normal = toGlmVec3(aimesh->mNormals[i]);
			normals[i] = normal;

			auto textureCoord = toGlmVec2(aimesh->HasTextureCoords(0) ? aimesh->mTextureCoords[0][i] : Zero3D );
			textures[i] = textureCoord;
		}

		for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
		{
			auto& face = aimesh->mFaces[i];
			triangles[i] = glm::uvec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
		}

		auto* mesh = new Mesh();
		mesh->setVertices(vertices, aimesh->mNumVertices);
		mesh->setNormals(normals, aimesh->mNumVertices);
		mesh->setTriangles(triangles, aimesh->mNumFaces);
		mesh->setTextures(textures, aimesh->mNumVertices);

		meshes.push_back(mesh);
	}

	return meshes;
}

GLuint Resources::loadTexture(const char* path) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 3) 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

GLuint Resources::loadCubemap(vector<string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

Material* Resources::loadMaterial(const char* vsPath, const char* fsPath)
{
	Shader* shader;

	shader = Shader::findVF(vsPath, fsPath);

	auto material = new Material();
	material->shader = shader;

	return material;
}

Resources * Resources::instance;