#pragma once
#include <Object.h>

#include <GL/glew.h>
#include <stdafx.h>

	class Shader : public Object
	{
	public:
		Shader();
		~Shader();

		static Shader *findEffect(const char* file, const char* programName = "Main");
		static Shader *findVF(const char* vertexShader, const char* fragmentShader);

		void use();
		GLint getUniformLocation(const char* uniformName);

	protected:
		void initFromEffect(const char* file, const char* programName = "Main");
		void initFromVF(const char* vertexShader, const char* fragmentShader);

		GLuint loadRawShader(const char* file, GLenum shaderType);
		void link();
		char* loadFile(const char* fileName);

		GLint effect = 0;
		GLint program = 0;

		static const int MAX_BUFFER_SIZE = 1048576; // 1MB
	};