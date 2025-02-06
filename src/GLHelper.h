#include <iostream>
#include <GL/glew.h>

namespace GLHelper
{
	static bool getError()
	{
		GLenum err = glGetError();
		if(err == GL_NO_ERROR) return true;
		while (err != GL_NO_ERROR) {
			switch (err) {
			case GL_INVALID_ENUM:
				Log::error(LogCategory::Render, "GL Error: GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				Log::error(LogCategory::Render, "GL Error: GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				Log::error(LogCategory::Render, "GL Error: GL_INVALID_OPERATION");
				break;
			case GL_STACK_OVERFLOW:
				Log::error(LogCategory::Render, "GL Error: GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				Log::error(LogCategory::Render, "GL Error: GL_STACK_UNDERFLOW");
				break;
			case GL_OUT_OF_MEMORY:
				Log::error(LogCategory::Render, "GL Error: GL_OUT_OF_MEMORY");
				break;
			default:
				Log::error(LogCategory::Render, "GL Error: Unknown error");
				break;
			}
			err = glGetError();  // Check for more errors
		}
		return false;
	}
}
