#pragma once

#include <Object.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include <stdafx.h>


enum BufferType
{
	Vertex,
	Normal,
	Texture,
	Color,
	Index,
	NumBuffers
};

enum TextureType 
{
	TwoD,
	Box
};

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	void setVertices(glm::vec3* value, int count);
	void setTriangles(glm::uvec3* value, int count);
	void setTextures(glm::vec2* value, int count);
	void setColors(glm::vec4* value, int count);
	void setNormals(glm::vec3* value, int count);
	void recalculateNormals(glm::vec3* vertices, int numVertices, glm::uvec3* triangles, int numTriangles);

	void bind();
	void bindTexture(TextureType type);

	void setTexture(GLuint id);

	int numTriangles;

protected:
	GLuint vao;
	GLuint texture;
	GLuint buffers[NumBuffers];
};
