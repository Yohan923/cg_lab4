#include "MeshFilter.h"

#include <GameObject.h>

#include <Mesh.h>

MeshFilter::MeshFilter()
{
}

MeshFilter::~MeshFilter()
{
	delete mesh;
}

void MeshFilter::beforeAttach(GameObject* gameObject)
{
	gameObject->meshFilters.push_back(this);
}
