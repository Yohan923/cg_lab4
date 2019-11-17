#pragma once

#include <stdafx.h>
#include <Component.h>


class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void draw() override;

	void setBackgroundColor(glm::vec4& value);
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateVP();

	static Camera* current;

	glm::vec3 cameraFront;

	float fov;
	float aspect;
	float nearClipPlane;
	float farClipPlane;
	float yaw;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 vpMatrix;
};

