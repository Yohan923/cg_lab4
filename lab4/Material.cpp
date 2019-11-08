#include "Material.h"

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

void Material::setWorldMatrix(const mat4& value)
{
	set(UniformWorldMatrix, value);
}

void Material::setCameraPos(const vec3& value)
{
	set(UniformCameraPos, value);
}

const char* Material::UniformColor = "_Color";
const char* Material::UniformWVP = "_MATRIX_MVP";
const char* Material::UniformWorldMatrix = "_Object2World";
const char* Material::UniformCameraPos = "_WorldSpaceCameraPos";