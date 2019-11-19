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

const char* simplePlanets[8][3] = 
{ 
	{"Earth", "resources/models/Earth/Earth.DAE", "resources/models/Earth/Textures/earthmap1k.jpg"},
	{"Jupiter", "resources/models/Jupiter/Jupiter.DAE", "resources/models/Jupiter/Textures/jupitermap.jpg"},
	{"Mars", "resources/models/Mars/Mars.DAE", "resources/models/Mars/Textures/mars_1k_color.jpg"},
	{"Uranus", "resources/models/Uranus/Uranus.DAE","resources/models/Uranus/Textures/uranusmap.jpg"},
	{"Neptune", "resources/models/Neptune/Neptune.DAE","resources/models/Neptune/Textures/neptunemap.jpg"},
	{"Venus", "resources/models/Venus/Venus.DAE","resources/models/Venus/Textures/venusmap.jpg"},
	{"Mercury", "resources/models/Mercury/Mercury.DAE","resources/models/Mercury/Textures/mercurymap.jpg"},
	{"Pluto", "resources/models/Pluto/Pluto.DAE","resources/models/Pluto/Textures/plutomap1k.jpg"},
};

const float simplePlanetsPresets[8][2] = 
{
	{100.0f, 0.4f},
	{150.0f, 0.8f},
	{175.0f, 0.3f},
	{250.0f, 0.4f},
	{300.0f, 0.35f},
	{75.0f, 0.1f},
	{50.0f, 0.1f},
	{350.0f, 0.1f},
};

void initAsteroids(Planet* earth)
{
	auto* asteroids = Resources::loadAsteroids("resources/models/asteroid/asteroid.obj", asteroidVS, asteroidFS);

	auto meshes = asteroids->meshFilters;
	meshes.at(0)->mesh->setTexture(Resources::loadTexture("resources/models/asteroid/texture.jpg"));
	
	SolarSystem::asteroids.push_back(asteroids);

	asteroids->transform->setParent(SolarSystem::sun->getObject()->transform);
}

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
	sun->name = "Sun";
	sun->rotationSpeed = 0.001f;

	//create saturn
	auto* saturn = Resources::loadGameObject("resources/models/Saturn/Saturn.DAE", commonVS, commonFS);
	saturn->transform->position = glm::vec3(0, 0, 0);
	saturn->transform->rotation = glm::quat(glm::vec3(-1.49, 0, 0));
	saturn->transform->scaling = sun->getObject()->transform->scaling * 0.7f;
	saturn->transform->setParent(sun->getObject()->transform);

	meshes = saturn->meshFilters;
	meshes.at(0)->mesh->setTexture(Resources::loadTexture("resources/models/Saturn/Textures/saturnmap.jpg"));
	meshes.at(1)->mesh->setTexture(Resources::loadTexture("resources/models/Saturn/Textures/saturnringcolor.jpg"));

	tmpPlanet = new Planet(saturn);
	tmpPlanet->name = "Saturn";
	tmpPlanet->rotationSpeed = RandomFloat(0.001f, 0.01f);
	tmpPlanet->orbitSpeed = RandomFloat(0.4f, 10.0f);
	tmpPlanet->orbitRadius = 200.0f;
	planets.push_back(tmpPlanet);

	Planet* earth = nullptr;

	//create rest of the simple planets
	for (int i = 0; i < 8; i++) 
	{
		auto* planet = Resources::loadGameObject(simplePlanets[i][1], commonVS, commonFS);
		planet->transform->position = glm::vec3(0, 0, 0);
		planet->transform->rotation = glm::quat(glm::vec3(-1.49, 0, 0));
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

		if (tmpPlanet->name == "Earth") 
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
	tmpPlanet->name = "Moon";
	tmpPlanet->rotationSpeed = RandomFloat(0.001f, 0.01f);
	tmpPlanet->orbitSpeed = 30.0f;
	tmpPlanet->orbitRadius = 30.0f;
	planets.push_back(tmpPlanet);

	initAsteroids(earth);
}

SolarSystem::~SolarSystem()
{
}

void SolarSystem::update() 
{
	if (Input::getKey('p'))
	{
		sun->rotationSpeed += 0.005;
	}
	if (Input::getKey('o'))
	{
		sun->rotationSpeed = max(sun->rotationSpeed - 0.005, 0.0);
	}

	float angularSpeed = sun->rotationSpeed;
	auto angle = (float)((angularSpeed * Input::deltaTime) * 180.0f / Pi);
	glm::quat newRotation = glm::angleAxis(angle, glm::vec3(0, 1, 0));
	sun->getObject()->transform->rotation = newRotation * sun->getObject()->transform->rotation;

	for (Planet* planet: planets)
	{
		float angularSpeed = planet->rotationSpeed;
		auto angle = (float)((angularSpeed * Input::deltaTime) * 180.0f / Pi);
		glm::quat newRotation = glm::angleAxis(angle, glm::vec3(0, 1, 1));
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

	for (int i = 0; i < asteroids.size(); i++) 
	{
		auto* asteroidObj = asteroids[i];
		auto* asteroidModel = asteroidsModels[i];

		for (auto* meshfilter : asteroidObj->meshFilters)
		{
			auto* mesh = meshfilter->mesh;
			updateAsteroidModel(asteroidObj, asteroidModel, mesh->numInstances);

			mesh->setInstances(asteroidModel, mesh->numInstances);
		}

	}

}

void SolarSystem::updateAsteroidModel(GameObject* asteroids, glm::mat4* asteroidModel, int size)
{


	auto parentModel = asteroids->transform->getParent()->transform->getModelMatrix();
	for (int i = 0; i < size; i++) 
	{
		asteroidModel[i] = parentModel * asteroidModel[i];
	}
}

vector<Planet*> SolarSystem::planets;
vector<GameObject*> SolarSystem::asteroids;
std::vector<glm::mat4*> SolarSystem::asteroidsModels;
Planet* SolarSystem::sun;