#include "Camera.h"
#include <stdafx.h>
#include <GameObject.h>
#include <Transform.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

void Camera::draw()
{
	viewMatrix = glm::lookAt(transform->position, transform->position + cameraFront, glm::vec3(0, 1, 0));
	vpMatrix = projectionMatrix * viewMatrix;
}

void Camera::setBackgroundColor(glm::vec4& value)
{
	glClearColor(value.x, value.y, value.z, value.w);
}

void Camera::updateViewMatrix()
{
	viewMatrix = glm::lookAt(transform->position, transform->position + cameraFront, glm::vec3(0, 1, 0));
}

void Camera::updateProjectionMatrix() 
{
	current->projectionMatrix = glm::perspective(glm::radians(current->fov), current->aspect, current->nearClipPlane, current->farClipPlane);
}

void Camera::updateVP() 
{
	current->updateViewMatrix();
	current->updateProjectionMatrix();
	vpMatrix = projectionMatrix * viewMatrix;
}

Camera::Camera()
{
	current = this;
}


Camera::~Camera()
{
}

Camera* Camera::current;
