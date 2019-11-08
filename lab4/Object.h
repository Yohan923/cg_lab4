#pragma once

#include <stdafx.h>

class Object
{
public:
	Object();
	~Object();

	virtual void update();
	virtual void draw();
};