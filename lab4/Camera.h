#pragma once

#include <stdafx.h>
#include <Component.h>


class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void draw() override;

	static Camera* current;

	glm::vec3 targetPos;

	float fov;
	float aspect;
	float nearClipPlane;
	float farClipPlane;

	glm::mat4 worldToCameraMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 vpMatrix;
};

