#include "ComputeShader.h"
#include <iostream>
#include "Log.h"
#include <sstream>

void ComputeShader::unload()
{
	glDeleteProgram(id);
}

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
	checkShaderErrors(computeShader, "compute");
}

void ComputeShader::createShaderProgram()
{
	// Create program
	id = glCreateProgram();
	glAttachShader(id, computeShader);
	glLinkProgram(id);
	
	glDeleteShader(computeShader);

	// Check for linking error
	int params = -1;
	glGetProgramiv(id, GL_LINK_STATUS, &params);
	if (params != GL_TRUE)
	{
		std::ostringstream s;
		s << "Could not link shader programme GL index " << id;
		Log::error(LogCategory::Render, s.str());
		printProgrammeInfoLog(id);
	}
	if (!isValid(id))
	{
		std::ostringstream s;
		s << "Could not validate compute shader " << id;
		Log::error(LogCategory::Render, s.str());
	}
}

void ComputeShader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}
void ComputeShader::setVector3f(const GLchar *name, const Vector3 &value)
{
	glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}

bool ComputeShader::isValid(GLuint id)
{
	glValidateProgram(id);
	int params = -1;
	glGetProgramiv(id, GL_VALIDATE_STATUS, &params);
	Log::info("");
	std::ostringstream s;
	s << "program " << id << " GL_VALIDATE_STATUS = " << params;
	Log::info(s.str());
	if (params != GL_TRUE)
	{
		printProgrammeInfoLog(id);
		return false;
	}
	return true;
}

void ComputeShader::printProgrammeInfoLog(GLuint id)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog(id, max_length, &actual_length, log);
	std::ostringstream s;
	s << "Program info log for GL index " << id;
	Log::info(s.str());
}

void ComputeShader::checkShaderErrors(GLuint shader, std::string shaderType)
{
	int params = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (params != GL_TRUE)
	{
		std::ostringstream s;
		s << "GL " << shaderType << " shader index " << shader << " did not compile.";
		Log::error(LogCategory::Render, s.str());
		printShaderInfoLog(shader);
	}
}

void ComputeShader::printShaderInfoLog(GLuint shaderIndex)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog(shaderIndex, max_length, &actual_length, log);
	std::ostringstream s;
	s << "Shader info log for GL index " << shaderIndex;
	Log::info(s.str());
}