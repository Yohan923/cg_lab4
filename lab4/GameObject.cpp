#include "GameObject.h"

#include <Component.h>
#include <Transform.h>

GameObject::GameObject()
{
	transform = new Transform();
	addComponent(transform);
}

GameObject::~GameObject()
{
	for (auto it = components.begin(); it != components.end();)
	{
		delete *it;
		it = components.erase(it);
	}
}

GameObject* GameObject::Instantiate(GameObject* original, glm::vec3& position, glm::quat &rotation)
{
	auto* gameObject = new GameObject(*original);
	gameObject->transform->position = position;
	gameObject->transform->rotation = rotation;

	return gameObject;
}

void GameObject::addComponent(Component* component)
{
	component->beforeAttach(this);
	component->gameObject = this;
	component->transform = transform;
	components.push_back(component);
	component->afterAttach(this);
}

Component* GameObject::find(std::string name)
{
	for (auto* component : components)
	{
		if (component->name == name) return component;
	}

	return nullptr;
}

void GameObject::update()
{
	for (auto* component : components)
	{
		if (component->enabled) component->update();
	}
}

void GameObject::draw()
{
	for (auto* component : components)
	{
		if (component->enabled) component->draw();
	}
}