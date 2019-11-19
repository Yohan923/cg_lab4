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
	void updateAsteroidModel(GameObject* asteroids, glm::mat4* asteroidModel, int size);

	static Planet* sun;
	static std::vector<Planet*> planets;
	static std::vector<GameObject*> asteroids;
	static std::vector<glm::mat4*> asteroidsModels;
};