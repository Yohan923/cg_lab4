#include "Planet.h"


Planet::Planet()
{
}

Planet::Planet(GameObject* gameObject)
{
	planet = gameObject;
	orbitAngle = 0;
}


Planet::~Planet()
{
}

GameObject* Planet::getObject() {
	return planet;
}