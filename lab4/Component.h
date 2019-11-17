#pragma once

#include <Object.h>
#include <stdafx.h>

class GameObject;
class Transform;

class Component : public Object
{
public:
	Component();
	~Component();

	virtual void beforeAttach(GameObject* gameObject);
	virtual void afterAttach(GameObject* gameObject);

	GameObject* gameObject;
	Transform* transform;

	bool enabled;
	std::string name;
};

