#pragma once
#include <Renderer.h>

class SkyBoxRenderer: public Renderer
{
public:
	SkyBoxRenderer();
	~SkyBoxRenderer();

	virtual void draw() override;
};

