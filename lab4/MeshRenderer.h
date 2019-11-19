#pragma once
#include <Renderer.h>

#include <stdafx.h>

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	MeshRenderer(const char* vsPath, const char* fsPath);
	~MeshRenderer();

	virtual void draw() override;
};
