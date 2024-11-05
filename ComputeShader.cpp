#include "ComputeShader.h"

ComputeShader &ComputeShader::use()
{
	glUseProgram(id);
	return *this;
}

void ComputeShader::compile(const GLchar *source)
{
	compileComputeShader(source);
	createShaderProgram();
}

void ComputeShader::compileComputeShader(const GLchar *source)
{
	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &source, nullptr);
	glCompileShader(computeShader);
}

void ComputeShader::createShaderProgram()
{
	// Create program
	id = glCreateProgram();
	glAttachShader(id, computeShader);
	glLinkProgram(id);
	
	glDeleteShader(computeShader);
}

void ComputeShader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}
void ComputeShader::setVector3f(const GLchar *name, const Vector3 &value)
{
	glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}