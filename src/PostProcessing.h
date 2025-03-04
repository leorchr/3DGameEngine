#pragma once
#include "Vector2.h"
#include "Window.h"
#include"GL/glew.h"
#include <vector>

class ComputeShader;
class Shader;

class PostProcessing
{
public:
	PostProcessing();
	bool initialize();
	void startDrawing();
	void computePostProcessing();
	void displayFrameBuffer() const;
	void setCustomFrambufferShader(Shader* shader);
	void setupComputeShader(ComputeShader* computeShader);

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

	// ImGui
public:
	void updateImGui();
	static void setPostProcessWindowActive(bool showRendererPostProcessWindow);

private:
	static int kernelSize;
	static int kernelAverage;
	static std::vector<std::vector<int>> kernel;
	static bool showPostProcessingWindow;
	static bool showPostProcessing;
	const int spacing = 2;
	bool mustComputeKernel = true;

};

void MyDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
						 GLsizei length, const GLchar* message,
						 const void* userParam);