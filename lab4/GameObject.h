#pragma once

#include <Object.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <stdafx.h>

class Component;
class Renderer;
class Transform;
class Mesh;

class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	static GameObject* Instantiate(GameObject* original, glm::vec3& position, glm::quat &rotation);

	void addComponent(Component* component);

	virtual void update() override;
	virtual void draw() override;

	Renderer* renderer;
	Transform* transform;
	Mesh* mesh;

	std::list<Renderer*> renderers;

protected:
	std::vector<Component*> components;
};