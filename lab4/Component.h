#pragma once

#include <Object.h>
#include <stdafx.h>

class GameObject;

class Component : public Object
{
public:
	Component();
	~Component();

	bool enabled;

	GameObject* gameObject;
};

