#include "CameraControl.h"

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Camera.h>
#include <Transform.h>
#include <Input.h>

using namespace glm;

CameraControl::CameraControl()
{
}

CameraControl::~CameraControl()
{
}

inline float acosDegree(float cos)
{
	const float Pi = (float)3.14159265358979323846;
	return (float)(acos(cos) * 180.0f / Pi);
}

void CameraControl::update()
{
	auto* camera = Camera::current;

	auto cameraPos = camera->transform->position;
	auto cameraFront = camera->cameraFront;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	auto pitch = camera->pitch;
	auto yaw = camera->yaw;

	float cameraSpeed = 80.0f * Input::deltaTime; // adjust accordingly

	if (Input::getKey('w')) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (Input::getKey('s')) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (Input::getKey('a')) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
	}
	if (Input::getKey('d')) {
		cameraPos += glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
	}
	if (Input::getKey('q')) {
		cameraPos += cameraUp * cameraSpeed;
	}
	if (Input::getKey('e')) {
		cameraPos -= cameraUp * cameraSpeed;
	}

	if (Input::curr.mouseDown[GLUT_LEFT_BUTTON] && Input::prev.mouseDown[GLUT_LEFT_BUTTON])
	{
		vec2 offset = Input::getMousePos() - Input::getPrevMousePos();
		if (length(offset) > 0) {
			float rotationSpeed = 5.0f * Input::deltaTime;  // degrees per pixel

			float xAngle = offset.x * rotationSpeed;
			float yAngle = offset.y * rotationSpeed;

			yaw += xAngle;
			pitch -= yAngle;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(front);
		}
	}

	const float ZoomSpeed = 50.0f * Input::deltaTime;

	if (Input::curr.wheel != 0)
	{
		float offset = Input::curr.wheel * ZoomSpeed;
		auto fov = camera->fov;
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= offset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
		camera->fov = fov;
	}

	camera->pitch = pitch;
	camera->yaw = yaw;
	camera->transform->position = cameraPos;
	camera->cameraFront = cameraFront;
	camera->updateVP();
}
