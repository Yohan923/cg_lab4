#pragma once

#include <Component.h>

#include <stdafx.h>

class Mesh;

class MeshFilter : public Component
{
public:
	MeshFilter();
	~MeshFilter();

	virtual void beforeAttach(GameObject* gameObject) override;

	Mesh *mesh;
};