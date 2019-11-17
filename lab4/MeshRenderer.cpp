#include "MeshRenderer.h"
#include <stdafx.h>
#include <Utils.h>
#include <Settings.h>

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


MeshRenderer::MeshRenderer(const char* vsPath, const char* fsPath)
{
	materials.push_back(Resources::loadMaterial(vsPath, fsPath));
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw()
{
	auto* material = getMaterial();
	auto* shader = material->shader;

	auto modelMatrix = transform->getModelMatrix();
	auto wvp = Camera::current->vpMatrix * modelMatrix;

	material->setWVP(wvp);
	material->setModelMatrix(modelMatrix);
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

		glDrawElements(
			GL_TRIANGLES,
			mesh->numTriangles * 3,
			GL_UNSIGNED_INT,
			0);
	}
}
