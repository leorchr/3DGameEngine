#include "PostProcessing.h"
#include "Assets.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "imgui.h"
#include "ImGUISettings.h"
#include "imgui_internal.h"
#include <GL/glew.h>
#include <iostream>

int PostProcessing::kernelSize = 5;
int PostProcessing::kernelAverage = 256;
std::vector<std::vector<int>> PostProcessing::kernel = {
	{1, 4, 6, 4, 1},
	{4, 16, 24, 16, 4},
	{6, 24, 36, 24, 6},
	{4, 16, 24, 16, 4},
	{1, 4, 6, 4, 1}
};;
bool PostProcessing::showPostProcessingWindow = false;
bool PostProcessing::showPostProcessing = false;

PostProcessing::PostProcessing() : FBO(0), rectVAO(0), rectVBO(0), frameBufferTexture(0), frameBufferOutputTexture(0), shader(nullptr), computeShader(nullptr){}

bool PostProcessing::initialize()
{
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
	
	glGenTextures(1, &frameBufferOutputTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferOutputTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum opengl = glGetError();
	if(opengl != GL_NO_ERROR)
	{
		std::cout<<"ERROR" << opengl;
		return false;
	}
	else{return true;}
	
}

void PostProcessing::startDrawing()
{
	if(!showPostProcessing) return;
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//Check si le buffer est valide
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Erreur de framebuffer : " << status << std::endl;
	}
	
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::computePostProcessing()
{
	if(!showPostProcessing) return;
	computeShader->use();
	
	glBindImageTexture(0, frameBufferTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(1, frameBufferOutputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	if(mustComputeKernel)
	{
		computeShader->setInteger("matrixSize", kernelSize);
		computeShader->setInteger("average", kernelAverage);
		computeShader->setMatrix(kernel, 2);
		mustComputeKernel = false;
	}

	// Exécute le compute shader
	int workgroupSizeX = 16;
	int workgroupSizeY = 16;
	glDispatchCompute(WINDOW_WIDTH/workgroupSizeX,WINDOW_HEIGHT/workgroupSizeY,1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void PostProcessing::displayFrameBuffer() const
{
	if(!showPostProcessing) return;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shader->use();
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindVertexArray(rectVAO);
	glActiveTexture(GL_TEXTURE0);
	shader->setInteger("screenTexture", 0);
	//shader->setVector2f("texelSize", texelSize);
	glBindTexture(GL_TEXTURE_2D, frameBufferOutputTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::setCustomFrambufferShader(Shader* shader)
{
	this->shader = shader;
}

void PostProcessing::setupComputeShader(ComputeShader* computeShader)
{
	this->computeShader = computeShader;
}

void PostProcessing::updateImGui()
{
	if(!showPostProcessingWindow) return;
	
	ImGui::SetNextWindowPos(ImGUISettings::computeShaderWindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImGUISettings::computeShaderWindowSize, ImGuiCond_Once);

	if(ImGui::Begin("Post Processing", &showPostProcessingWindow, ImGuiWindowFlags_NoNavFocus))
	
	ImGui::Checkbox("Blur Effect", &showPostProcessing);
	
	if(ImGui::DragInt("Kernel Size", &kernelSize, 2, 3, 6, "%d"))
	{
		kernelSize = kernelSize % 2 == 0 ? kernelSize + 1 : kernelSize;
		kernel.resize(kernelSize);
		for(auto& row : kernel)
		{
			row.resize(kernelSize);
		}
		mustComputeKernel = true;
	}

	if(ImGui::DragInt("Kernel Average", &kernelAverage, 1, 1, 10000, "%d"))
	{
		mustComputeKernel = true;	
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 windowPadding = window->WindowPadding;
	int padding = (int)windowPadding.x;
	
	for(size_t i = 0; i < kernel.size(); i++)
	{
		for(size_t j = 0; j < kernel[0].size(); j++)
		{
			int& value = kernel[i][j];
			std::string uniqueLabel = "##CurrentKernelMatrixValue" + std::to_string(i) + std::to_string(j);
			ImGui::PushItemWidth(((int)ImGui::GetWindowWidth() - padding*2 - spacing * (kernel.size()-1)) / kernel.size());
			if(ImGui::DragInt(uniqueLabel.c_str(), &value))
			{
				mustComputeKernel = true;
			}
			ImGui::PopItemWidth();
			if(j >= kernelSize-1) continue;
			ImGui::SameLine(0, (int)spacing);
		}
	}	
	ImGui::End();
}

void PostProcessing::setPostProcessWindowActive(bool showRendererPostProcessWindow)
{
	showPostProcessingWindow = showRendererPostProcessWindow;
}