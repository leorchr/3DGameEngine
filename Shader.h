#pragma once 
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

#include "Vector2.h"
#include "Matrix4.h"

// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
	// State
	GLuint id;

	// Constructor
	Shader(): id(0), vs(0), fs(0), gs(0), tcs(0), tes(0) { }

	// Unload the shader
	void unload();

	// Sets the current shader as active
	Shader& use();

	// Compiles the shader from given source code
	void compile(
		const GLchar* vertexSource,
		const GLchar* fragmentSource,
		const GLchar* tessControlSource = nullptr,
		const GLchar* tessEvalSource = nullptr,
		const GLchar* geometrySource = nullptr
	);

	// Utility functions
	void setFloat(const GLchar* name, GLfloat value);
	void setInteger(const GLchar* name, GLint value);
	void setVector2f(const GLchar* name, GLfloat x, GLfloat y);
	void setVector2f(const GLchar* name, const Vector2& value);
	void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void setVector3f(const GLchar* name, const Vector3& value);
	void setMatrix4(const GLchar* name, const Matrix4& matrix,const bool transpose);
	void setName(std::string name);

private:
	GLuint vs;
	GLuint fs;
	GLuint tcs;
	GLuint tes;
	GLuint gs;
	std::string name;

	void compileVertexShader(const GLchar* vertexSource);
	void compileFragmentShader(const GLchar* fragmentSource);
	bool compileTessControlShader(const GLchar* tessControlSource);
	bool compileTessEvalShader(const GLchar* tessEvalSource);
	bool compileGeometryShader(const GLchar* geometrySource);
	void createShaderProgram(bool tessShadersExist, bool geometryShaderExists);

	void checkShaderErrors(GLuint shader, std::string shaderType);
	void printShaderInfoLog(GLuint shaderIndex);
	void printProgrammeInfoLog(GLuint programme);
	void printAllParams(GLuint programme, bool vertexExist, bool fragmentExist, bool tessControlExist, bool tessEvalExist, bool geometryExist);
	bool isValid(GLuint programme);
};

const char* GLTypeToString(GLenum type);
const char* GLShaderTypeToString(GLenum type);
