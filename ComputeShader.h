#pragma once
#include <GL/glew.h>
#include "Vector3.h"

class ComputeShader
{
public:
	GLuint id;

	ComputeShader() { }

	ComputeShader& use();

	void compile(const GLchar* source);

	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
	void setVector3f(const GLchar *name, const Vector3 &value);

private:
	GLuint computeShader;
	void compileComputeShader(const GLchar *cSource);
	void createShaderProgram();
};

