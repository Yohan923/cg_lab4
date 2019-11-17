#pragma once
#include <stdafx.h>
#include <Component.h>
#include <list>

class Transform : public Component
{
public:
	Transform();
	~Transform();

	virtual void beforeAttach(GameObject* gameObject) override;

	virtual void update() override;
	virtual void draw() override;

	Transform* getParent();
	void setParent(Transform* value);
	
	void clearChildren();

	glm::mat4 getModelMatrix();

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scaling;

	static std::list<Transform*> roots;

protected:
	Transform* parent;
	std::list<Transform*> children;
};

