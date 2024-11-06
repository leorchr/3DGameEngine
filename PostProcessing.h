#pragma once
class ComputeShader;
class Shader;
class PostProcessing
{
public:
	PostProcessing();
	bool initialize();
	void startDrawing() const;
	void displayFrameBuffer() const;

private:
	unsigned int FBO;
	unsigned int rectVAO, rectVBO;
	unsigned int RBO;
	unsigned int frameBufferTexture;
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
};
