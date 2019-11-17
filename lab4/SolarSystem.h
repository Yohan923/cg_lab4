#pragma once
#include <map>
#include <Component.h>

class GameObject;
class Planet;

class SolarSystem : public Component
{
public:
	SolarSystem();
	~SolarSystem();

	virtual void update() override;

	static Planet* sun;
	static std::vector<Planet*> planets;
};