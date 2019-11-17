#include "SkyBoxRenderer.h"
#include <Settings.h>

#include <Material.h>
#include <Shader.h>
#include <Camera.h>
#include <GameObject.h>
#include <Transform.h>
#include <MeshFilter.h>
#include <Mesh.h>
#include <Resources.h>


SkyBoxRenderer::SkyBoxRenderer()
{
	materials.push_back(Resources::loadMaterial(skyboxVS, skyboxFS));
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

void SkyBoxRenderer::draw() {
	auto* material = getMaterial();
	auto* shader = material->shader;

	auto modelMatrix = transform->getModelMatrix();

	Camera::current->updateVP();

	glm::mat4 view = glm::mat4(glm::mat3(Camera::current->viewMatrix));

	material->setViewMatrix(view);
	material->setProjMatrix(Camera::current->projectionMatrix);

	auto meshes = gameObject->meshFilters;
	for (auto meshFilter : meshes) {
		GLint OldCullFaceMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
		GLint OldDepthFuncMode;
		glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		auto mesh = meshFilter->mesh;
		mesh->bind();
		mesh->bindTexture(Box);

		glDrawElements(
			GL_TRIANGLES,
			mesh->numTriangles * 3,
			GL_UNSIGNED_INT,
			0);

		glCullFace(OldCullFaceMode);
		glDepthFunc(OldDepthFuncMode);

	}
}
