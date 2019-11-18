#include <map>
#include <glm/gtx/quaternion.hpp>
#include <Utils.h>

#include "SolarSystem.h"
#include <Resources.h>
#include <GameObject.h>
#include <Transform.h>
#include <MeshFilter.h>
#include <Mesh.h>
#include <Material.h>
#include <Renderer.h>
#include <Input.h>
#include <Planet.h>

const char* simplePlanets[3][3] = 
{ 
	{"earth", "resources/models/Earth/Earth.DAE", "resources/models/Earth/Textures/earthmap1k.jpg"},
	{"jupiter", "resources/models/Jupiter/Jupiter.DAE", "resources/models/Jupiter/Textures/jupitermap.jpg"},
	{"mars", "resources/models/Mars/Mars.DAE", "resources/models/Mars/Textures/mars_1k_color.jpg"},
};

const float simplePlanetsPresets[4][2] = 
{
	{100.0f, 0.4f},
	{300.0f, 0.8f},
	{200.0f, 0.3f},
	{600.0f, 0.1f}
};

SolarSystem::SolarSystem()
{
	Planet* tmpPlanet;

	//create sun
	auto* sunObj = Resources::loadGameObject("resources/models/Sun/Sun.DAE", sunVS, sunFS);
	sunObj->transform->position = glm::vec3(0, 0, 0);
	sunObj->transform->rotation = glm::quat(glm::vec3(0, 0, 0));
	sunObj->transform->scaling = glm::vec3(1.0, 1.0, 1.0);

	auto meshes = sunObj->meshFilters;
	meshes.at(0)->mesh->setTexture(Resources::loadTexture("resources/models/Sun/Textures/2k_sun.jpg"));

	sun = new Planet(sunObj);
	sun->name = "sun";
	sun->rotationSpeed = 0.005f;

	//create saturn
	auto* saturn = Resources::loadGameObject("resources/models/Saturn/Saturn.DAE", commonVS, commonFS);
	saturn->transform->position = glm::vec3(0, 0, 0);
	saturn->transform->rotation = glm::quat(glm::vec3(0, 0, 0));
	saturn->transform->scaling = sun->getObject()->transform->scaling * 0.7f;
	saturn->transform->setParent(sun->getObject()->transform);

	meshes = saturn->meshFilters;
	meshes.at(0)->mesh->setTexture(Resources::loadTexture("resources/models/Saturn/Textures/saturnmap.jpg"));
	meshes.at(1)->mesh->setTexture(Resources::loadTexture("resources/models/Saturn/Textures/saturnringcolor.jpg"));

	tmpPlanet = new Planet(saturn);
	tmpPlanet->name = "saturn";
	tmpPlanet->rotationSpeed = RandomFloat(0.001f, 0.01f);
	tmpPlanet->orbitSpeed = RandomFloat(0.4f, 10.0f);
	tmpPlanet->orbitRadius = 400.0f;
	planets.push_back(tmpPlanet);

	Planet* earth = nullptr;

	//create rest of the simple planets
	for (int i = 0; i < 3; i++) 
	{
		auto* planet = Resources::loadGameObject(simplePlanets[i][1], commonVS, commonFS);
		planet->transform->position = glm::vec3(0, 0, 0);
		planet->transform->rotation = glm::quat(glm::vec3(0, 0, 0));
		planet->transform->scaling = sun->getObject()->transform->scaling * simplePlanetsPresets[i][1];
		planet->transform->setParent(sun->getObject()->transform);

		auto meshes = planet->meshFilters;
		meshes.at(0)->mesh->setTexture(Resources::loadTexture(simplePlanets[i][2]));

		tmpPlanet = new Planet(planet);
		tmpPlanet->name = simplePlanets[i][0];
		tmpPlanet->rotationSpeed = RandomFloat(0.001f, 0.01f);
		tmpPlanet->orbitSpeed = RandomFloat(0.4f, 10.0f);
		tmpPlanet->orbitRadius = simplePlanetsPresets[i][0];
		planets.push_back(tmpPlanet);

		if (tmpPlanet->name == "earth") 
		{
			earth = tmpPlanet;
		}
	}

	//create earth moon
	auto* moon = Resources::loadGameObject("resources/models/Moon/Moon.DAE", commonVS, commonFS);
	moon->transform->position = glm::vec3(0, 0, 0);
	moon->transform->rotation = glm::quat(glm::vec3(0, 0, 0));
	moon->transform->scaling = earth->getObject()->transform->scaling * 0.5f;
	moon->transform->setParent(earth->getObject()->transform);

	meshes = moon->meshFilters;
	meshes.at(0)->mesh->setTexture(Resources::loadTexture("resources/models/Moon/Textures/moonmap2k.jpg"));

	tmpPlanet = new Planet(moon);
	tmpPlanet->name = "moon";
	tmpPlanet->rotationSpeed = RandomFloat(0.001f, 0.01f);
	tmpPlanet->orbitSpeed = 30.0f;
	tmpPlanet->orbitRadius = 30.0f;
	planets.push_back(tmpPlanet);
}

SolarSystem::~SolarSystem()
{
}

void SolarSystem::update() 
{
	for (Planet* planet: planets)
	{
		float angularSpeed = planet->rotationSpeed;
		auto angle = (float)((angularSpeed * Input::deltaTime) * 180.0f / Pi);
		glm::quat newRotation = glm::angleAxis(angle, glm::vec3(0, 1, 0));
		planet->getObject()->transform->rotation = newRotation * planet->getObject()->transform->rotation;
	}

	for (Planet* planet : planets)
	{
		float angularSpeed = planet->orbitSpeed;
		auto* parent = planet->getObject()->transform->getParent();
	
		planet->orbitAngle += Input::deltaTime * angularSpeed;
		float x = planet->orbitRadius * sin(Pi * 2 * planet->orbitAngle / 360);
		float y = planet->orbitRadius * cos(Pi * 2 * planet->orbitAngle / 360);
	
		planet->getObject()->transform->position = glm::vec3(x, 0.0f, y);
	}
}

vector<Planet*> SolarSystem::planets;
Planet* SolarSystem::sun;