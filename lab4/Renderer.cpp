#include "Renderer.h"
#include <stdafx.h>

#include <GameObject.h>

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::beforeAttach(GameObject* gameObject)
{
	if (gameObject->renderers.size() == 0) {
		gameObject->renderer = this;
	}

	gameObject->renderers.push_back(this);
}