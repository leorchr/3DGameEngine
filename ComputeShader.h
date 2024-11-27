#pragma once
#include <GL/glew.h>
#include "Vector3.h"
#include "Vector2.h"
#include <string>

class ComputeShader
{
public:
	GLuint id;

	ComputeShader(): id(0), cs(0) { }

	void unload();
	ComputeShader& use();

	void compile(const GLchar* source);

	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);
	void setVector3f(const GLchar *name, const Vector3 &value);
	void setVector2i(const GLchar* name, GLint x, GLint y, GLint z);
	void setVector2i(const GLchar *name, const Vector2 &value);
	void setVector2f(const GLchar* name, const Vector2& value);
	void setName(std::string name);

private:
	GLuint cs;
	std::string name;
	void compileComputeShader(const GLchar *cSource);
	void createShaderProgram();

	bool isValid(GLuint programme);
	void printProgrammeInfoLog(GLuint id);
	void checkShaderErrors(GLuint shader, std::string shaderType);
	void printShaderInfoLog(GLuint shaderIndex);
	void printAllParams();
};

