#include "Resources.h"
#include <stdafx.h>
#include <iostream>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <MeshRenderer.h>
#include <MeshFilter.h>
#include <GameObject.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <SkyBoxRenderer.h>
#include <InstanceRenderer.h>
#include <SolarSystem.h>

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

GameObject* Resources::loadAsteroids(const char* path, const char* vsPath, const char* fsPath)
{
	unsigned int amount = 10000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 400.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.01 and 0.11f
		float scale = (rand() % 1) / 1000.0f + 0.0005;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	vector<Mesh*> meshes = loadMeshes(path);

	auto* gameObject = new GameObject();

	for (Mesh* mesh : meshes) {
		mesh->setInstances(modelMatrices, amount);
		auto* meshFilter = new MeshFilter();
		meshFilter->mesh = mesh;
		gameObject->addComponent(meshFilter);
	}

	auto* renderer = new InstanceRenderer(vsPath, fsPath);
	gameObject->addComponent(renderer);

	SolarSystem::asteroidsModels.push_back(modelMatrices);

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