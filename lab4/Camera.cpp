#include <Camera.h>
#include <stdafx.h>
#include <GameObject.h>
#include <Transform.h>

void Camera::draw()
{
	worldToCameraMatrix = glm::lookAt(gameObject->transform->position, targetPos, glm::vec3(0, 1, 0));
	vpMatrix = projectionMatrix * worldToCameraMatrix;
}


Camera::Camera()
{
	current = this;
}


Camera::~Camera()
{
}

Camera* Camera::current;
