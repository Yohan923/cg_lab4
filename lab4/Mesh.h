#pragma once

#include <Object.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stdafx.h>


enum BufferType
{
	Vertex,
	Normal,
	Color,
	Index,
	NumBuffers
};

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	void setVertices(glm::vec3* value, int count);
	void setTriangles(glm::uvec3* value, int count);
	void setColors(glm::vec4* value, int count);
	void setNormals(glm::vec3* value, int count);
	void recalculateNormals(glm::vec3* vertices, int numVertices, glm::uvec3* triangles, int numTriangles);

	void bind();

	int numTriangles;

protected:
	GLuint vao;
	GLuint buffers[NumBuffers];
};
