#include "Shader.h"

#include <iostream>

#include <gl/glfx.h>

Shader::Shader()
{
}

Shader::~Shader()
{
	if (program != 0) {
		glDeleteProgram(program);
		program = 0;
	}

	if (effect != 0) {
		glfxDeleteEffect(effect);
		effect = 0;
	}
}

Shader* Shader::findEffect(const char* file, const char* programName)
{
	auto* shader = new Shader();
	shader->initFromEffect(file, programName);
	return shader;
}

Shader* Shader::findVF(const char* vertexShader, const char* fragmentShader)
{
	auto* shader = new Shader();
	shader->initFromVF(vertexShader, fragmentShader);
	return shader;
}

void Shader::initFromEffect(const char* file, const char* programName)
{
	effect = glfxGenEffect();

	if (!glfxParseEffectFromFile(effect, file))
	{
		const char* log = glfxGetEffectLog(effect);
		fprintf(stderr, "Error creating effect from file '%s':\n", file);
		fprintf(stderr, "%s\n", log);
		throw 1;
	}

	program = glfxCompileProgram(effect, programName);

	if (program < 0)
	{
		const char* log = glfxGetEffectLog(effect);
		fprintf(stderr, "Error compiling program '%s' in effect file '%s':\n", programName, file);
		fprintf(stderr, "%s\n", log);
		throw 1;
	}
}

void Shader::initFromVF(const char* vertexShader, const char* fragmentShader)
{
	program = glCreateProgram();
	if (!program) {
		fprintf(stderr, "Error creating shader program\n");
		throw 1;
	}

	auto vs = loadRawShader(vertexShader, GL_VERTEX_SHADER);
	auto fs = loadRawShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	link();
}

GLuint Shader::loadRawShader(const char* file, GLenum shaderType)
{
	// create a shader object
	auto id = glCreateShader(shaderType);

	if (!id) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		throw 1;
	}

	char* shaderText = loadFile(file);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(id, 1, (const GLchar**)&shaderText, NULL);

	delete shaderText;

	// compile the shader and check for errors
	glCompileShader(id);

	// check for shader related errors using glGetShaderiv
	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		const int ErrorLogLength = 1024;
		GLchar errorLog[ErrorLogLength];
		glGetShaderInfoLog(id, ErrorLogLength, NULL, errorLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, errorLog);
		throw 1;
	}

	return id;
}

char* Shader::loadFile(const char* fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName, "rb");

	if (!fp) {
		fprintf(stderr, "Error: failed loading file '%s'\n", fileName);
		throw 1;
	}

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);

	if (fileSize >= MAX_BUFFER_SIZE)
	{
		fprintf(stderr, "Error: file '%s' (%d B) is larger than max buffer size (%d B)\n",
			fileName, fileSize, MAX_BUFFER_SIZE);
		throw 1;
	}

	char* buffer = new char[fileSize + 1];
	fseek(fp, 0L, SEEK_SET);

	fread(buffer, fileSize, 1, fp);
	buffer[fileSize] = 0;

	fclose(fp);

	return buffer;
}

void Shader::link()
{
	GLint success = 0;
	GLchar errorLog[1024];

	// After compiling all shader objects and attaching them to the program,
	// we can finally link it
	glLinkProgram(program);

	// check for program related errors using glGetProgramiv
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		throw 1;
	}

	// program has been successfully linked but needs to be validated
	// to check whether the program can execute given the current pipeline state
	glValidateProgram(program);

	// check for program related errors using glGetProgramiv
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		throw 1;
	}
}

void Shader::use()
{
	glUseProgram(program);
}

GLint Shader::getUniformLocation(const char* uniformName)
{
	use();

	auto location = glGetUniformLocation(program, uniformName);
	const int InvalidUniformLocation = -1;

	if (location == InvalidUniformLocation)
	{
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniformName);
	}

	return location;
}
