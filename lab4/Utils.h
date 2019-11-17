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

static float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

inline glm::vec3 transformPosition(const glm::vec3& v, const glm::mat4& t)
{
	return glm::vec3(t * glm::vec4(v, 1));
}