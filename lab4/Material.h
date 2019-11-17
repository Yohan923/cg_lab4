#pragma once
#include <Object.h>
#include <glm/glm.hpp>
#include <stdafx.h>
#include <Utils.h>


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
	void setModelMatrix(const glm::mat4& value);
	void setViewMatrix(const glm::mat4& value);
	void setProjMatrix(const glm::mat4& value);
	void setCameraPos(const glm::vec3& value);
	void setLightPosition(const glm::vec3& value);
	void setLightColor(const glm::vec3& value);
	void setAmbientIntensity(float value);
	void setDiffuseIntensity(float value);
	void setSpecularIntensity(float value);
	void setSpecularPower(float value);
	void setAttenuationConst(float value);
	void setAttenuationLinear(float value);
	void setAttenuationExp(float value);

	void setBaseLight(BaseLight light);
	void setPointLight(PointLight light);

	static const char* UniformColor;
	static const char* UniformWVP;
	static const char* UniformModelMatrix;
	static const char* UniformViewMatrix;
	static const char* UniformProjMatrix;
	static const char* UniformCameraPos;
	static const char* UniformLightPosition;
	static const char* UniformLightColor;
	static const char* UniformAmbientIntensity;
	static const char* UniformDiffuseIntensity;
	static const char* UniformSpecularIntensity;
	static const char* UniformSpecularPower;
	static const char* UniformAttenuationConst;
	static const char* UniformAttenuationLinear;
	static const char* UniformAttenuationExp;
};

