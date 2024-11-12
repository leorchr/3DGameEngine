#pragma once
#include <GL/glew.h>
#include "Vector3.h"
#include "Vector2.h"
#include <xstring>

class ComputeShader
{
public:
	GLuint id;

	ComputeShader(): id(0), computeShader(0) { }

	void unload();
	ComputeShader& use();

	void compile(const GLchar* source);

	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
	void setVector3f(const GLchar *name, const Vector3 &value);
	void setVector2i(const GLchar* name, GLint x, GLint y, GLint z);
	void setVector2i(const GLchar *name, const Vector2 &value);

private:
	GLuint computeShader;
	void compileComputeShader(const GLchar *cSource);
	void createShaderProgram();

	bool isValid(GLuint programme);
	void printProgrammeInfoLog(GLuint id);
	void checkShaderErrors(GLuint shader, std::string shaderType);
	void printShaderInfoLog(GLuint shaderIndex);
};

