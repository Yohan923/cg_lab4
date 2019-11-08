#pragma once
#include <Object.h>
#include <glm/glm.hpp>
#include <stdafx.h>


class Shader;

class Material : public Object
{
public:
	Material();
	~Material();

	Shader *shader;

	void set(const char* propertyName, const glm::vec3& value);
	void set(const char* propertyName, const glm::vec4& value);
	void set(const char* propertyName, const glm::mat3& value);
	void set(const char* propertyName, const glm::mat4& value);
	void set(const char* propertyName, float value);

	void setColor(const glm::vec3& value);
	void setColor(const glm::vec4& value);
	void setWVP(const glm::mat4& value);
	void setWorldMatrix(const glm::mat4& value);
	void setCameraPos(const glm::vec3& value);

	static const char* UniformColor;
	static const char* UniformWVP;
	static const char* UniformWorldMatrix;
	static const char* UniformCameraPos;
};

