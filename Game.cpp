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
#include "PlayerActor.h"
#include <algorithm>

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
	Assets::loadTexture(renderer, "Res\\Textures\\Radar.png", "Cube");
	Assets::loadTexture(renderer, "Res\\Textures\\HealthBar.png", "HealthBar");
	Assets::loadTexture(renderer, "Res\\Textures\\Planch.png", "Plane");
	Assets::loadTexture(renderer, "Res\\Textures\\Radar.png", "Radar");
	Assets::loadTexture(renderer, "Res\\Textures\\Sphere.png", "Sphere");
	Assets::loadTexture(renderer, "Res\\Textures\\Crosshair.png", "Crosshair");
	Assets::loadTexture(renderer, "Res\\Textures\\RacingCar.png", "RacingCar");
	Assets::loadTexture(renderer, "Res\\Textures\\Rifle.png", "Rifle");
	Assets::loadTexture(renderer, "Res\\Textures\\Target.png", "Target");

	Assets::loadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");
	Assets::loadMesh("Res\\Meshes\\Rifle.gpmesh", "Mesh_Rifle");
	Assets::loadMesh("Res\\Meshes\\RacingCar.gpmesh", "Mesh_RacingCar");
	Assets::loadMesh("Res\\Meshes\\Target.gpmesh", "Mesh_Target");

	player = new PlayerActor();
	stationary2 = new StationaryActor(Maths::toRadians(40.0f), Maths::toRadians(-30.0f));
	stationary2->setPosition(Vector3(650, 100, 100));
	stationary3 = new StationaryActor(Maths::pi/2);
	stationary3->setPosition(Vector3(850, 0, 200));
	fps = new FPSActor();


	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi*2));

	// Pins
	const float sizePins = 17.0f;
	const float spacePins = 17.0f;
	const float scaleZ = 45.0f;

	int numberPins = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < j+1; i++) {
			pins.push_back(new PinActor());
			pins[numberPins]->setPosition(Vector3(800.0f + sizePins * j + spacePins * j, i * sizePins + spacePins * i - sizePins * j, -100.0f + scaleZ / 2));
			pins[numberPins]->setScale(Vector3(sizePins, sizePins, scaleZ));
			//pins[numberPins]->setRotation(q);
			numberPins++;
		}
	}

	// Floor and walls

	// Setup floor
	const float start = 0.0f;
	const float sizePlane = 50.0f;
	for (int i = 0; i < 11; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->setPosition(Vector3(start + i * sizePlane * p->getScale().x, start, -100.0f));
	}

	// Left/right walls
	q = Quaternion(Vector3::unitX, Maths::piOver2);
	for (int i = 0; i < 20; i++)
	{
		PlaneActor* p = new PlaneActor();
		p->setPosition(Vector3(start + i * sizePlane, 15 + start - sizePlane*3, -170.0f));
		p->setRotation(q);

		p = new PlaneActor();
		p->setPosition(Vector3(start + i * sizePlane, -15 -start + sizePlane*3, -170.0f));
		p->setRotation(q);
	}

	//const float sizeWall = 100.0f;
	//q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::piOver2));
	//// Forward/back walls
	//for (int i = 0; i < 21; i++)
	//{
	//	for (int j = 0; j < 21; j++)
	//	{
	//		PlaneActor* p = new PlaneActor();
	//		p->setPosition(Vector3(1200 + start - sizeWall, -1000 + start + i * sizeWall, -1000 + j *sizeWall));
	//		p->setRotation(q);
	//	}
	//}

	// Setup lights
	renderer.setAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = renderer.getDirectionalLight();
	dir.direction = Vector3(0.5f, -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

	// Crosshair
	Actor* crosshairActor = new Actor();
	crosshairActor->setScale(Vector3(2.0f, 2.0f, 2.0f));
	crosshair = new SpriteComponent(crosshairActor, Assets::getTexture("Crosshair"));

	changeCamera(1);
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
	if (input.keyboard.getKeyState(SDL_SCANCODE_1) == ButtonState::Pressed)
	{
		changeCamera(1);
	}
	else if (input.keyboard.getKeyState(SDL_SCANCODE_2) == ButtonState::Pressed)
	{
		changeCamera(2);
	}
	else if (input.keyboard.getKeyState(SDL_SCANCODE_3) == ButtonState::Pressed)
	{
		changeCamera(3);
	}
	else if (input.keyboard.getKeyState(SDL_SCANCODE_4) == ButtonState::Pressed)
	{
		changeCamera(4);
	}

	// Actor input
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->processInput(input);
	}
	isUpdatingActors = false;
}

void Game::changeCamera(int mode)
{
	// Disable everything
	player->setState(Actor::ActorState::Paused);
	player->setVisible(false);
	stationary2->setState(Actor::ActorState::Paused);
	stationary3->setState(Actor::ActorState::Paused);
	fps->setState(Actor::ActorState::Paused);
	fps->setVisible(false);
	crosshair->setVisible(false);

	// Enable the camera specified by the mode
	switch (mode)
	{
	case 1:
	default:
		player->setState(Actor::ActorState::Active);
		player->setCameraOrientation();
		player->setVisible(true);
		break;
	case 2:
		stationary2->setState(Actor::ActorState::Active);
		stationary2->setCameraOrientation();
		break;
	case 3:
		stationary3->setState(Actor::ActorState::Active);
		stationary3->setCameraOrientation();
		break;
	case 4:
		fps->setState(Actor::ActorState::Active);
		break;
	}
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
