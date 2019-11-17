#pragma once

#include <stdafx.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

namespace Assimp
{
	class Importer;
}

class GameObject;
class Material;
class Mesh;
class SkyBox;

class Resources
{
public:
	Resources();
	~Resources();

	static GameObject* loadGameObject(const char* path, const char* vsPath, const char* fsPath);
	static GameObject* loadSkyBox();
	static vector<Mesh*> loadMeshes(const char* path);
	static GLuint loadTexture(const char* path);
	static Material* loadMaterial(const char* vsPath, const char* fsPath);
	static GLuint loadCubemap(vector<string> faces);


	static Resources *instance;
};
