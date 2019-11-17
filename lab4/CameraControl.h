#pragma once

#include <Component.h>

#include <stdafx.h>

class CameraControl: public Component
{
public:
	CameraControl();
	~CameraControl();

	virtual void update() override;

protected:
	glm::vec2 prevPos;
};
