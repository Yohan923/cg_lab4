#pragma once
#include <stdafx.h>

const float Pi = (float)3.14159265358979323846;

struct BaseLight 
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	float SpecularIntensity;
	float SpecularPower;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight
{
	BaseLight baseLight;
	glm::vec3 Position;
	Attenuation attenuation;
};

static BaseLight baseLight =
{
   glm::vec3(1),
   0.55f,
   0.6f,
   1.0f,
   1.0f
};

static Attenuation attenuation =
{
   1.0f,
   0.0014f,
   0.000007f,
};

static const char* commonVS = "resources/shaders/simpleVertexShader.txt";
static const char* commonFS = "resources/shaders/simpleFragmentShader.txt";

static const char* sunVS = "resources/shaders/sunVS.txt";
static const char* sunFS = "resources/shaders/sunFS.txt";

static const char* skyboxVS = "resources/shaders/skyBoxVS.txt";
static const char* skyboxFS = "resources/shaders/skyBoxFS.txt";

static const char* asteroidVS = "resources/shaders/asteroidVS.txt";
static const char* asteroidFS = "resources/shaders/asteroidFS.txt";

inline float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

inline glm::vec3 transformPosition(const glm::vec3& v, const glm::mat4& t)
{
	return glm::vec3(t * glm::vec4(v, 1));
}

inline glm::vec3 toGlmVec3(const aiVector3D &v)
{
	return glm::vec3(v.x, v.y, v.z);
}

inline glm::vec2 toGlmVec2(const aiVector3D &v)
{
	return glm::vec2(v.x, v.y);
}