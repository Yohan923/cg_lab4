#include "Material.h"
#include <Utils.h>

#include <GL/glew.h>
#include <gl/glfx.h>
#include <glm/gtc/type_ptr.hpp>

#include <Material.h>
#include <Shader.h>

using namespace glm;

Material::Material()
{
}

Material::~Material()
{
	delete shader;
}

void Material::set(const char* propertyName, const vec3& value)
{
	shader->use();
	auto location = shader->getUniformLocation(propertyName);
	glUniform3fv(location, 1, value_ptr(value));
}

void Material::set(const char* propertyName, const vec4& value)
{
	shader->use();
	auto location = shader->getUniformLocation(propertyName);
	glUniform4fv(location, 1, value_ptr(value));
}

void Material::set(const char* propertyName, const mat3& value)
{
	shader->use();
	auto location = shader->getUniformLocation(propertyName);
	glUniformMatrix3fv(location, 1, false, value_ptr(value));
}

void Material::set(const char* propertyName, const mat4& value)
{
	shader->use();
	auto location = shader->getUniformLocation(propertyName);
	glUniformMatrix4fv(location, 1, false, value_ptr(value));
}

void Material::set(const char* propertyName, float value)
{
	shader->use();
	auto location = shader->getUniformLocation(propertyName);
	glUniform1f(location, value);
}

void Material::setColor(const vec3& value)
{
	set(UniformColor, vec4(value, 1));
}

void Material::setColor(const vec4& value)
{
	set(UniformColor, value);
}

void Material::setWVP(const mat4& value)
{
	set(UniformWVP, value);
}

void Material::setModelMatrix(const mat4& value)
{
	set(UniformModelMatrix, value);
}

void Material::setViewMatrix(const mat4& value)
{
	set(UniformViewMatrix, value);
}

void Material::setProjMatrix(const mat4& value)
{
	set(UniformProjMatrix, value);
}


void Material::setCameraPos(const vec3& value)
{
	set(UniformCameraPos, value);
}

void Material::setLightPosition(const vec3& value)
{
	set(UniformLightPosition, value);
}

void Material::setLightColor(const vec3& value)
{
	set(UniformLightColor, value);
}

void Material::setAmbientIntensity(float value)
{
	set(UniformAmbientIntensity, value);
}

void Material::setDiffuseIntensity(float value)
{
	set(UniformDiffuseIntensity, value);
}

void Material::setSpecularIntensity(float value)
{
	set(UniformSpecularIntensity, value);
}

void Material::setSpecularPower(float value)
{
	set(UniformSpecularPower, value);
}

void Material::setAttenuationConst(float value)
{
	set(UniformAttenuationConst, value);
}
void Material::setAttenuationLinear(float value)
{
	set(UniformAttenuationLinear, value);
}
void Material::setAttenuationExp(float value)
{
	set(UniformAttenuationExp, value);
}

void Material::setBaseLight(BaseLight light)
{
	setLightColor(light.Color);
	setAmbientIntensity(light.AmbientIntensity);
	setDiffuseIntensity(light.DiffuseIntensity);
	setSpecularIntensity(light.SpecularIntensity);
	setSpecularPower(light.SpecularPower);
}
void Material::setPointLight(PointLight light)
{ 
	setBaseLight(light.baseLight);
	setAttenuationConst(light.attenuation.Constant);
	setAttenuationLinear(light.attenuation.Linear);
	setAttenuationExp(light.attenuation.Exp);
}

const char* Material::UniformColor = "_Color";
const char* Material::UniformWVP = "_MATRIX_MVP";
const char* Material::UniformModelMatrix = "_ModelMatrix";
const char* Material::UniformViewMatrix = "_ViewMatrix";
const char* Material::UniformProjMatrix = "_ProjMatrix";
const char* Material::UniformCameraPos = "_WorldSpaceCameraPos";
const char* Material::UniformLightPosition = "_PointLight.Position";
const char* Material::UniformLightColor = "_BaseLight.Color";
const char* Material::UniformAmbientIntensity = "_BaseLight.AmbientIntensity";
const char* Material::UniformDiffuseIntensity = "_BaseLight.DiffuseIntensity";
const char* Material::UniformSpecularIntensity = "_BaseLight.SpecularIntensity";
const char* Material::UniformSpecularPower = "_BaseLight.SpecularPower";
const char* Material::UniformAttenuationConst = "_PointLight.Attenuation.Constant";
const char* Material::UniformAttenuationLinear = "_PointLight.Attenuation.Linear";
const char* Material::UniformAttenuationExp = "_PointLight.Attenuation.Exp";