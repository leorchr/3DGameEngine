#include "ComputeShader.h"
#include "Log.h"
#include <iostream>
#include "Shader.h"
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
	printAllParams();
}

void ComputeShader::compileComputeShader(const GLchar *source)
{
	cs = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(cs, 1, &source, nullptr);
	glCompileShader(cs);
	checkShaderErrors(cs, "compute");
}

void ComputeShader::createShaderProgram()
{
	// Create program
	id = glCreateProgram();
	glAttachShader(id, cs);
	glLinkProgram(id);
	
	glDeleteShader(cs);

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

void ComputeShader::setVector2i(const GLchar *name, GLint x, GLint y, GLint z)
{
	glUniform2i(glGetUniformLocation(id, name), x, y);
}

void ComputeShader::setVector2i(const GLchar *name, const Vector2 &value)
{
	glUniform2i(glGetUniformLocation(id, name), value.x, value.y);
}

void ComputeShader::setVector2f(const GLchar *name, const Vector2 &value)
{
	glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

bool ComputeShader::isValid(GLuint id)
{
	glValidateProgram(id);
	int params = -1;
	glGetProgramiv(id, GL_VALIDATE_STATUS, &params);
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

void ComputeShader::printAllParams()
{
    Log::info("-----------------------------");

    std::ostringstream outputName;
    outputName << "Compute shader name : " << name;
    Log::info(outputName.str());
    
    std::ostringstream s;
    s << "Compute shader program " << id << " info: ";
    Log::info(s.str());

    
    int params = -1;
    glGetProgramiv(id, GL_LINK_STATUS, &params);
    s.str("");
    s << "GL_LINK_STATUS = " << params;
    Log::info(s.str());

    glGetProgramiv(id, GL_ATTACHED_SHADERS, &params);
    s.str("");
    s << "GL_ATTACHED_SHADERS = " << params;
    Log::info(s.str());
	
    s.str("");
    GLint shaderType = 0;
    glGetShaderiv(cs, GL_SHADER_TYPE, &shaderType);
    s << "  0) Type: Compute Shader\n";
    Log::info(s.str());
	
    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &params);
    s.str("");
    s << "GL_ACTIVE_ATTRIBUTES = " << params;
    Log::info(s.str());
    for (GLuint i = 0; i < (GLuint)params; i++)
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib(id, i, max_length, &actual_length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[77];
                sprintf_s(long_name, "%s[%i]", name, j);
                int location = glGetAttribLocation(id, long_name);
                std::ostringstream s;
                s << "  " << i << ") Type:" << GLTypeToString(type) << " Name:" << long_name << " Location:" << location;
                Log::info(s.str());
            }
        }
        else
        {
            int location = glGetAttribLocation(id, name);
            std::ostringstream s;
            s << "  " << i << ") Type:" << GLTypeToString(type) << " Name:" << name << " Location:" << location;
            Log::info(s.str());
        }
    }

    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &params);
    s.str("");
    s << "GL_ACTIVE_UNIFORMS = " << params;
    Log::info(s.str());
    for (GLuint i = 0; i < (GLuint)params; i++)
    {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(id, i, max_length, &actual_length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[77];
                sprintf_s(long_name, "%s[%i]", name, j);
                int location = glGetUniformLocation(id, long_name);
                std::ostringstream s;
                s << "  " << i << ") Type: " << GLTypeToString(type) << " | Name: " << long_name << " | Location: " << location;
                Log::info(s.str());
            }
        }
        else
        {
            int location = glGetUniformLocation(id, name);
            std::ostringstream s;
            s << "  " << i << ") Type: " << GLTypeToString(type) << " | Name: " << name << " | Location: " << location;
            Log::info(s.str());
        }
    }
}

void ComputeShader::setName(std::string name)
{
	this->name = name;
}
