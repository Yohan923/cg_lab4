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
class MeshFilter;
class MeshRenderer;
class Camera;

class GameObject : public Object
{
public:
	GameObject();
	~GameObject();

	static GameObject* Instantiate(GameObject* original, glm::vec3& position, glm::quat &rotation);

	void addComponent(Component* component);
	Component* find(std::string name);

	virtual void update() override;
	virtual void draw() override;

	Renderer* renderer;
	Transform* transform;
	std::vector<MeshFilter*> meshFilters;

	std::list<Renderer*> renderers;

protected:
	std::vector<Component*> components;
};