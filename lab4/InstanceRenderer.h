#pragma once
#include <MeshRenderer.h>


class InstanceRenderer : public MeshRenderer
{
public:
	InstanceRenderer(const char* vsPath, const char* fsPath);
	~InstanceRenderer();

	virtual void draw() override;
};

