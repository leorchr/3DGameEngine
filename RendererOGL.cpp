#include "RendererOGL.h"
#include "Rectangle.h"
#include "Assets.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Log.h"
#include "Game.h"
#include "UIScreen.h"
#include <complex>
#include <GL/glew.h>
#include <SDL_image.h>
#include "ComputeShader.h"
#include <iostream>
#include "ImGUIManager.h"

RendererOGL::RendererOGL() :
	window(nullptr),
	context(nullptr),
	spriteVertexArray(nullptr),
	spriteViewProj(Matrix4::createSimpleViewProj(WINDOW_WIDTH, WINDOW_HEIGHT)),
	view(Matrix4::createLookAt(Vector3::zero, Vector3::unitX, Vector3::unitZ)),
	projection(Matrix4::createPerspectiveFOV(Maths::toRadians(70.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f)),
	ambientLight(Vector3(1.0f, 1.0f, 1.0f)),
	dirLight({Vector3::zero, Vector3::zero, Vector3::zero}),
	postProcessing(nullptr) {}

RendererOGL::~RendererOGL() {}

bool RendererOGL::initialize(Window& windowP, bool usePostProcessing = false)
{
	window = &windowP;

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Depth buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Enable debugging
	glEnable(GL_DEBUG_OUTPUT);

	// OpenGL Context
	context = SDL_GL_CreateContext(windowP.getSDLWindow());
	SDL_SetWindowFullscreen(windowP.getSDLWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);

	// GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		Log::error(LogCategory::Video, "Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code, so clear it
	glGetError();

	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}

	spriteVertexArray = new VertexArray(spriteVertices, 4, indices, 6);
	if(usePostProcessing)
	{
		postProcessing = new PostProcessing();
		postProcessing->initialize();
	}
	return true;
}

void RendererOGL::beginDraw()
{
	//Nettoie base buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Erreur de framebuffer : " << status << std::endl;
	}
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//Nettoie et active custom buffer
	if(postProcessing) postProcessing->startDrawing();
	ImGUIManager::beginDraw();
}

void RendererOGL::draw()
{
	//Draw on the custom buffer
	drawMeshes();
	drawSprites();
	drawUI();
}

void RendererOGL::endDraw()
{
	ImGUIManager::render();
	if(postProcessing) postProcessing->displayFrameBuffer();
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::close()
{
	delete spriteVertexArray;
	SDL_GL_DeleteContext(context);
}

Vector3 RendererOGL::unproject(const Vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= WINDOW_WIDTH * 0.5f;
	deviceCoord.y /= WINDOW_HEIGHT * 0.5f;

	// Transform vector by unprojection matrix
	Matrix4 unprojection = view * projection;
	unprojection.invert();
	return Vector3::transformWithPerspDiv(deviceCoord, unprojection);
}

void RendererOGL::getScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = unproject(screenPoint);
	// Get direction vector
	outDir = end - outStart;
	outDir.normalize();
}

void RendererOGL::drawMeshes()
{
	// Disable alpha blend
	glDisable(GL_BLEND);
	Shader& shader = Assets::getShader("Mesh");
	shader.use();
	// Update view-projection matrix
	Matrix4 viewProj = view * projection;
	shader.setMatrix4("uViewProjection", viewProj, false);
	// Lights
	setLightUniforms(shader);
	// Draw
	for(auto mc : meshes)
	{
		if(mc->getVisible())
		{
			mc->draw(Assets::getShader("Mesh"));
		}
	}
}

void RendererOGL::addSprite(SpriteComponent* sprite)
{
	// Insert the sprite at the right place in function of drawOrder
	int spriteDrawOrder = sprite->getDrawOrder();
	auto iter = begin(sprites);
	for(; iter != end(sprites); ++iter)
	{
		if(spriteDrawOrder < (*iter)->getDrawOrder()) break;
	}
	sprites.insert(iter, sprite);
}

void RendererOGL::removeSprite(SpriteComponent* sprite)
{
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	sprites.erase(iter);
}

void RendererOGL::drawSprites()
{
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Active shader and vertex array
	Shader& spriteShader = Assets::getShader("Sprite");
	spriteShader.use();
	spriteShader.setMatrix4("uViewProj", spriteViewProj, true);
	spriteVertexArray->setActive();

	for(auto sprite : sprites)
	{
		if(sprite->getVisible())
		{
			sprite->draw(*this);
		}
	}
}

void RendererOGL::drawUI()
{
	for(auto ui : Game::instance().getUIStack())
	{
		ui->draw(Assets::getShader("Sprite"));
	}
}

void RendererOGL::drawSprite(const Actor& actor, const Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const
{
	Matrix4 scaleMat = Matrix4::createScale((float)tex.getWidth(), (float)tex.getHeight(), 1.0f);
	Matrix4 world = scaleMat * actor.getWorldTransform();
	Assets::getShader("Sprite").setMatrix4("uWorldTransform", world, true);
	tex.bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::addMesh(MeshComponent* mesh)
{
	meshes.emplace_back(mesh);
}

void RendererOGL::removeMesh(MeshComponent* mesh)
{
	auto iter = std::find(begin(meshes), end(meshes), mesh);
	meshes.erase(iter);
}

void RendererOGL::setViewMatrix(const Matrix4& viewP)
{
	view = viewP;
}

void RendererOGL::setLightUniforms(Shader& shader)
{
	// Camera position is from inverted view
	Matrix4 invertedView = view;
	invertedView.invert();
	shader.setVector3f("uCameraPos", invertedView.getTranslation());
	// Ambient
	shader.setVector3f("uAmbientLight", ambientLight);
	// Directional light
	shader.setVector3f("uDirLight.direction", dirLight.direction);
	shader.setVector3f("uDirLight.diffuseColor", dirLight.diffuseColor);
	shader.setVector3f("uDirLight.specColor", dirLight.specColor);
}

void RendererOGL::setAmbientLight(const Vector3& ambientP)
{
	ambientLight = ambientP;
}