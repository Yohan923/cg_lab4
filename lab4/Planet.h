#pragma once
#include <GameObject.h>

class Planet
{
public:
	Planet();
	Planet(GameObject* gameObject);
	~Planet();

	GameObject* getObject();

	float orbitSpeed;
	float rotationSpeed;
	float orbitAngle;
	float orbitRadius;
	std::string name;
private:
	GameObject* planet;
};

