#include "InstanceRenderer.h"
#include <Resources.h>
#include <Mesh.h>
#include <MeshFilter.h>
#include <Transform.h>
#include <GameObject.h>
#include <Material.h>
#include <Shader.h>
#include <Camera.h>
#include <Resources.h>
#include <SolarSystem.h>
#include <Planet.h>



InstanceRenderer::InstanceRenderer(const char* vsPath, const char* fsPath)
{
	materials.push_back(Resources::loadMaterial(vsPath, fsPath));
}


InstanceRenderer::~InstanceRenderer()
{
}

void InstanceRenderer::draw()
{
	auto* material = getMaterial();
	auto* shader = material->shader;

	auto viewMatrix = Camera::current->viewMatrix;
	auto projMatrix = Camera::current->projectionMatrix;

	material->setViewMatrix(viewMatrix);
	material->setProjMatrix(projMatrix);
	material->setCameraPos(Camera::current->transform->position);

	PointLight pointLight =
	{
		baseLight,
		SolarSystem::sun->getObject()->transform->position,
		attenuation
	};
	material->setPointLight(pointLight);

	auto meshes = gameObject->meshFilters;
	for (auto meshFilter : meshes) {
		auto mesh = meshFilter->mesh;
		mesh->bind();
		mesh->bindTexture(TwoD);

		glDrawElementsInstanced(GL_TRIANGLES, mesh->numTriangles * 3, GL_UNSIGNED_INT, 0, mesh->numInstances);
	}
}
