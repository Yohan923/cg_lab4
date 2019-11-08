#pragma once
#include <Component.h>
#include <list>
#include <stdafx.h>

class GameObject;
class Material;

class Renderer : public Component
{
public:
	Renderer();
	~Renderer();

	virtual void draw() override;

	Material* getMaterial() { return materials.front(); }

	std::list<Material*> materials;
};

