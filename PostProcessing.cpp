#include "PostProcessing.h"
#include "Assets.h"
#include "Shader.h"
#include "ComputeShader.h"
#include <GL/glew.h>
#include "Window.h"
#include <iostream>

PostProcessing::PostProcessing() : FBO(0), rectVAO(0), rectVBO(0), frameBufferTexture(0), shader(nullptr), computeShader(nullptr){}

bool PostProcessing::initialize()
{
	Assets::loadComputeShader("Ressources/Shaders/Filter.glsl", "Filter");
	computeShader = &Assets::getComputeShader("Filter");
	computeShader->use();
	
	Assets::loadShader("Ressources/Shaders/Framebuffer.vert", "Ressources/Shaders/Framebuffer.frag", "", "", "", "FrameBuffer");
	shader = &Assets::getShader("FrameBuffer");
	shader->use();
	shader->setInteger("screenTexture", 0);


	// Prepare framebuffer rectangle VBO and VAO
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
	
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	GLenum DrawBuffer[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffer);

	GLenum opengl = glGetError();
	if(opengl != GL_NO_ERROR)
	{
		std::cout<<"ERROR" << opengl;
		return false;
	}
	else{return true;}
}

void PostProcessing::startDrawing() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Erreur de framebuffer : " << status << std::endl;
	}
}

void PostProcessing::displayFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	computeShader->use();

	glBindImageTexture(0, frameBufferTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(1, frameBufferTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	// Exécute le compute shader
	int workgroupSizeX = 16;
	int workgroupSizeY = 16;
	glDispatchCompute(WINDOW_WIDTH/workgroupSizeX,WINDOW_HEIGHT/workgroupSizeY,1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
	shader->use();
	glBindVertexArray(rectVAO);
	glActiveTexture(GL_TEXTURE0);
	shader->setInteger("screenTexture", 0);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}