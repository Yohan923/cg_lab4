#include "Transform.h"
#include <list>
#include <GameObject.h>

Transform::Transform()
	: scaling(1, 1, 1)
{
	roots.push_back(this);
}

Transform::~Transform()
{
	clearChildren();

	auto& siblings = parent == nullptr ? roots : parent->children;

	for (auto it = siblings.begin(); it != siblings.end();)
	{
		if (*it == this) {
			it = siblings.erase(it);
			break;
		}
		else {
			++it;
		}
	}
}

void Transform::update()
{
	for (auto* child : children)
	{
		child->gameObject->update();
	}
}

void Transform::draw()
{
	for (auto* child : children)
	{
		child->gameObject->draw();
	}
}

Transform* Transform::getParent()
{
	return parent;
}

void Transform::setParent(Transform* value)
{
	auto& siblings = parent == nullptr ? roots : parent->children;

	for (auto it = siblings.begin(); it != siblings.end();)
	{
		if (*it == this) {
			it = siblings.erase(it);
			break;
		}
		else {
			++it;
		}
	}

	auto& newSiblings = value == nullptr ? roots : value->children;
	newSiblings.push_back(this);

	parent = value;
}

void Transform::clearChildren()
{
	for (auto it = children.begin(); it != children.end();)
	{
		delete *(it++);
	}
}

glm::mat4 Transform::getWorldMatrix()
{
	if (parent != nullptr) {
		auto parentWorldMatrix = parent->getWorldMatrix();
		auto translateMatrix = glm::translate(position);
		auto rotationMatrix = toMat4(rotation);
		auto scalingMatrix = scale(this->scaling);
		return parentWorldMatrix * translateMatrix * rotationMatrix * scalingMatrix;
	}
	else {
		auto translateMatrix = translate(position);
		auto rotationMatrix = toMat4(rotation);
		auto scalingMatrix = scale(this->scaling);
		return translateMatrix * rotationMatrix * scalingMatrix;
	}
}

std::list<Transform*> Transform::roots;