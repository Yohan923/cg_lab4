#include "Mesh.h"

using namespace glm;

Mesh::Mesh()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(NumBuffers, buffers);
}

Mesh::~Mesh()
{
	glBindVertexArray(vao);
	glDeleteBuffers(NumBuffers, buffers);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::setVertices(vec3* value, int count)
{
	int size = sizeof(vec3) * count;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertex]);
	glBufferData(GL_ARRAY_BUFFER, size, value, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Vertex);
	glVertexAttribPointer(Vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		throw err;
	}
}

void Mesh::setTextures(vec2* value, int count)
{
	int size = sizeof(vec2) * count;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Texture]);
	glBufferData(GL_ARRAY_BUFFER, size, value, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Texture);
	glVertexAttribPointer(Texture, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		throw err;
	}
}

void Mesh::setTriangles(uvec3* value, int count)
{
	int size = sizeof(uvec3) * count;
	numTriangles = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, value, GL_STATIC_DRAW);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		throw err;
	}
}

void Mesh::setColors(vec4* value, int count)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Color]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * count, value, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Color);
	glVertexAttribPointer(Color, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::recalculateNormals(vec3* vertices, int numVertices, uvec3* triangles, int numTriangles)
{
	vec3* normals = new vec3[numVertices];
	int numIndices = numTriangles * 3;

	for (int i = 0; i < numTriangles; ++i)
	{
		uvec3& t = triangles[i];

		vec3 side1 = vertices[t.y] - vertices[t.x];
		vec3 side2 = vertices[t.y] - vertices[t.z];

		vec3 normal = cross(side1, side2);
		normals[t.x] = length(normal) > 0 ? normalize(normal) : vec3();

		normals[t.y] = normals[t.x];
		normals[t.z] = normals[t.x];
	}

	int size = sizeof(vec3) * numVertices;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Normal]);
	glBufferData(GL_ARRAY_BUFFER, size, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Normal);
	glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		throw err;
	}

	delete[] normals;
}

void Mesh::bind()
{
	glBindVertexArray(vao);
}

void Mesh::bindTexture(TextureType type) {
	if (type == TwoD) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else if (type == Box) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}
}

void Mesh::setNormals(glm::vec3* value, int count)
{
	int size = sizeof(vec3) * count;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Normal]);
	glBufferData(GL_ARRAY_BUFFER, size, value, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Normal);
	glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		throw err;
	}
}

void Mesh::setTexture(GLuint id) {
	bind();
	texture = id;
}

void Mesh::setInstances(glm::mat4* value, int count)
{
	bind();
	numInstances = count;

	glBindBuffer(GL_ARRAY_BUFFER, buffers[Instances]);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), &value[0], GL_STATIC_DRAW);
	// set attribute pointers for matrix (4 times vec4)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		throw err;
	}
}