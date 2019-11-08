#include "Renderer.h"
#include <stdafx.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <GameObject.h>
#include <Mesh.h>
#include <Camera.h>
#include <Transform.h>
#include <Shader.h>
#include <Material.h>

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::draw()
{
	auto* material = getMaterial();
	auto* shader = material->shader;

	auto worldMatrix = gameObject->transform->getWorldMatrix();
	auto wvp = Camera::current->vpMatrix * worldMatrix;

	material->setWVP(wvp);
	material->setWorldMatrix(worldMatrix);
	material->setCameraPos(Camera::current->gameObject->transform->position);

	auto* mesh = gameObject->mesh;
	mesh->bind();

	glDrawElements(
		GL_TRIANGLES,
		mesh->numTriangles * 3,
		GL_UNSIGNED_INT,
		0);
}
