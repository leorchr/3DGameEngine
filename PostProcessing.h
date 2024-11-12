#pragma once
#include "Vector2.h"
#include "Window.h"
#include"GL/glew.h"

class ComputeShader;
class Shader;

class PostProcessing
{
public:
	PostProcessing();
	bool initialize();
	void startDrawing();
	void displayFrameBuffer();

private:
	unsigned int FBO;
	unsigned int rectVAO, rectVBO;
	unsigned int RBO;
	unsigned int frameBufferTexture;
	unsigned int frameBufferOutputTexture;
	Shader* shader;
	ComputeShader* computeShader;
	
	const float rectangleVertices[24] = {
		// Coords    // texCoords
		1.0f, -1.0f,  1.0f, 0.0f,
	   -1.0f, -1.0f,  0.0f, 0.0f,
	   -1.0f,  1.0f,  0.0f, 1.0f,

		1.0f,  1.0f,  1.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
	   -1.0f,  1.0f,  0.0f, 1.0f
   };

	const Vector2 texelSize = Vector2(1.0f/WINDOW_WIDTH,1.0f/WINDOW_HEIGHT);
};

void MyDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
						 GLsizei length, const GLchar* message,
						 const void* userParam);