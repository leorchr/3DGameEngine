#include "Game.h"
#include "Actor.h"
#include "Timer.h"
#include "Assets.h"
#include "MeshComponent.h"
#include "CubeActor.h"
#include "SphereActor.h"
#include "PlaneActor.h"
#include "FPSActor.h"
#include "StationaryActor.h"
#include "SplineActor.h"
#include "TargetActor.h"
#include <algorithm>
#include <iostream>
using namespace std;

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);
	bool isInputInit = inputSystem.initialize();

	return isWindowInit && isRendererInit && isInputInit; // Return bool && bool && bool ...to detect error
}

void Game::load()
{
	inputSystem.setMouseRelativeMode(true);

	Assets::loadShader("Res\\Shaders\\Sprite.vert", "Res\\Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("Res\\Shaders\\Phong.vert", "Res\\Shaders\\Phong.frag", "", "", "", "Phong");
	Assets::loadShader("Res\\Shaders\\BasicMesh.vert", "Res\\Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");

	Assets::loadTexture(renderer, "Res\\Textures\\Default.png", "Default");
	Assets::loadTexture(renderer, "Res\\Textures\\Pin.png", "Cube");
	Assets::loadTexture(renderer, "Res\\Textures\\HealthBar.png", "HealthBar");
	Assets::loadTexture(renderer, "Res\\Textures\\Planch.png", "Plane");
	Assets::loadTexture(renderer, "Res\\Textures\\Radar.png", "Radar");
	Assets::loadTexture(renderer, "Res\\Textures\\Ball.png", "Sphere");
	Assets::loadTexture(renderer, "Res\\Textures\\Crosshair.png", "Crosshair");
	Assets::loadTexture(renderer, "Res\\Textures\\RacingCar.png", "RacingCar");
	Assets::loadTexture(renderer, "Res\\Textures\\Rifle.png", "Rifle");
	Assets::loadTexture(renderer, "Res\\Textures\\Target.png", "Target");
	Assets::loadTexture(renderer, "Res\\Textures\\Ground.jpg", "Ground");
	Assets::loadTexture(renderer, "Res\\Textures\\Wall.jpg", "Wall");

	Assets::loadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");
	Assets::loadMesh("Res\\Meshes\\Rifle.gpmesh", "Mesh_Rifle");
	Assets::loadMesh("Res\\Meshes\\RacingCar.gpmesh", "Mesh_RacingCar");
	Assets::loadMesh("Res\\Meshes\\Target.gpmesh", "Mesh_Target");

	fps = new FPSActor();

	// Corsshair
	Actor* crosshairActor = new Actor();
	crosshairActor->setScale(Vector3(2.0f,2.0f,2.0f));
	SpriteComponent* crosshair = new SpriteComponent(crosshairActor, Assets::getTexture("Crosshair"));


	{
	Quaternion q2(Vector3::unitY, Maths::piOver2);
	q2 = Quaternion::concatenate(q2, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi * 2));
	PlaneActor* p = new PlaneActor();
	p->getMesh()->setTextureIndex(0);
	p->setScale(Vector3(2, 0.1f, 1));
	p->setPosition(Vector3(750, 350, 0));
	p->setRotation(q2);

	PlaneActor* p2 = new PlaneActor();
	p2->getMesh()->setTextureIndex(0);
	p2->setScale(Vector3(2, 0.1f, 1));
	p2->setPosition(Vector3(750, 450, 0));
	p2->setRotation(q2);

	// Floor and walls
	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi*2));

	// Setup floor
	const float start = 0.0f;
	const float sizePlane = 100.0f;
	for (int i = 0; i < 11; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, start, -101.0f));
	}

	// Setup Back Wall
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			PlaneActor* p = new PlaneActor();
			p->getMesh()->setTextureIndex(2);
			p->setPosition(Vector3(1200, -2000 + sizePlane * i * 2, -50.0f + sizePlane * j * 2));
			p->setRotation(q2);
		}
	}


	//Setup Gutters
	for (int i = 0; i < 11; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->getMesh()->setTextureIndex(1);
		p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, -35 + start, -110.0f));
	}

	for (int i = 0; i < 11; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->getMesh()->setTextureIndex(1);
		p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, 35 + start, -110.0f));
	}


	const float start2 = 235.0f;
	//Setup Ground
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			PlaneActor* p = new PlaneActor();
			p->getMesh()->setTextureIndex(1);
			p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, start2 + j * sizePlane * 2, -70.0f));
		}	
	}

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			PlaneActor* p = new PlaneActor();
			p->getMesh()->setTextureIndex(1);
			p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, -start2 - j * sizePlane * 2, -70.0f));
		}
	}


	// Left/right walls
	q = Quaternion(Vector3::unitX, Maths::piOver2);
	for (int i = 0; i < 11; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, 15 + start - sizePlane * 1.5f, -170.0f));
		p->setRotation(q);

		p = new PlaneActor();
		p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, -15 -start + sizePlane * 1.5f, -170.0f));
		p->setRotation(q);
	}

	// Setup lights
	renderer.setAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
	DirectionalLight& dir = renderer.getDirectionalLight();
	dir.direction = Vector3(0.5f, 0.2f, -0.7f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

	}
}

void Game::processInput()
{
	inputSystem.preUpdate();

	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		isRunning = inputSystem.processEvent(event);
	}

	inputSystem.update();
	const InputState& input = inputSystem.getInputState();

	// Escape: quit game
	if (input.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
	{
		isRunning = false;
	}

	// Actor input
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->processInput(input);
	}
	isUpdatingActors = false;
}

void Game::update(float dt)
{
	// Update actors 
	isUpdatingActors = true;
	for(auto actor: actors) 
	{
		actor->update(dt);
	}
	isUpdatingActors = false;

	// Move pending actors to actors
	for (auto pendingActor: pendingActors)
	{
		pendingActor->computeWorldTransform();
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	// Delete dead actors
	vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->getState() == Actor::ActorState::Dead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}
}

void Game::render()
{
	renderer.beginDraw();
	renderer.draw();
	renderer.endDraw();
}

void Game::loop()
{
	Timer timer;
	float dt = 0;
	while (isRunning)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::unload()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Resources
	Assets::clear();
}

void Game::close()
{
	inputSystem.close();
	renderer.close();
	window.close();
	SDL_Quit();
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from the two vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}

void Game::addPlane(PlaneActor* plane)
{
	planes.emplace_back(plane);
}

void Game::removePlane(PlaneActor* plane)
{
	auto iter = std::find(begin(planes), end(planes), plane);
	planes.erase(iter);
}