#include "Component.h"



Component::Component() : Object()
{
	enabled = true;
}


Component::~Component()
{
}

void Component::beforeAttach(GameObject* gameObject)
{
}

void Component::afterAttach(GameObject* gameObject)
{

}
