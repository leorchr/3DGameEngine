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
#include "DoorActor.h"
#include "LockedDoorActor.h"
#include "PauseScreen.h"
#include "KeyActor.h"
#include "MovingPlatformActor.h"
#include "HitPoints.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);
	bool isInputInit = inputSystem.initialize();
	bool isFontInit = Font::initialize();

	return isWindowInit && isRendererInit && isInputInit && isFontInit; // Return bool && bool && bool ...to detect error
}

void Game::load()
{
	inputSystem.setMouseRelativeMode(true);

	Assets::loadShader("Res\\Shaders\\Sprite.vert", "Res\\Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("Res\\Shaders\\Phong.vert", "Res\\Shaders\\Phong.frag", "", "", "", "Phong");
	Assets::loadShader("Res\\Shaders\\BasicMesh.vert", "Res\\Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");

	Assets::loadTexture(renderer, "Res\\Textures\\Wall.jpg", "Cube");
	Assets::loadTexture(renderer, "Res\\Textures\\Planch.png", "Plane");
	Assets::loadTexture(renderer, "Res\\Textures\\Radar.png", "Radar");
	Assets::loadTexture(renderer, "Res\\Textures\\Rouge.jpg", "Sphere");
	Assets::loadTexture(renderer, "Res\\Textures\\Crosshair.png", "Crosshair");
	Assets::loadTexture(renderer, "Res\\Textures\\Rifle.png", "Rifle");
	Assets::loadTexture(renderer, "Res\\Textures\\Ground.jpg", "Ground");
	Assets::loadTexture(renderer, "Res\\Textures\\Wall.jpg", "Wall");
	Assets::loadTexture(renderer, "Res\\Textures\\WallIco.png", "WallIco");
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonYellow.png", "ButtonYellow");
	Assets::loadTexture(renderer, "Res\\Textures\\ButtonBlue.png", "ButtonBlue");
	Assets::loadTexture(renderer, "Res\\Textures\\RadarArrow.png", "RadarArrow");
	Assets::loadTexture(renderer, "Res\\Textures\\Blackbg.jpg", "Blackbg");
	Assets::loadTexture(renderer, "Res\\Textures\\Jaune.jpg", "Jaune");
	Assets::loadTexture(renderer, "Res\\Textures\\Rouge.jpg", "Rouge");
	Assets::loadTexture(renderer, "Res\\Textures\\spikeTrap.png", "Spikes");

	Assets::loadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");
	Assets::loadMesh("Res\\Meshes\\Rifle.gpmesh", "Mesh_Rifle");
	Assets::loadMesh("Res\\Meshes\\Key.gpmesh", "Mesh_Key");
	Assets::loadMesh("Res\\Meshes\\LockedDoor.gpmesh", "Mesh_LockedDoor");

	Assets::loadFont("Res\\Fonts\\Carlito-Regular.ttf", "Carlito");
	Assets::loadText("Res\\Localization\\English.gptext");

	Assets::loadMap("Res\\Maps\\map.json", "BaseMap");

	fps = new FPSActor();
	hud = new HUD();
	hitPoints = new HitPoints();

	{

	Quaternion q(Vector3::unitY, Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi * 2));

	std::vector<std::vector<int>> map = Assets::getMap("BaseMap");

	for (int i = 0; i < map.size(); i++)
	{
		std::vector<int> invertMap = map[i];
		std::reverse(invertMap.begin(), invertMap.end());
		for (int y = 0; y < map[i].size(); y++)
		{
			if (invertMap[y] == 1) {
				Actor* cube = new CubeActor();
				cube->setScale(Vector3(500, 500, 500));
				cube->setPosition(Vector3(500 * i, 500 * y, 150));
				cube->setRotation(q);
			}
			else if (invertMap[y] == 2) {
				fps->setPosition(Vector3(500 * i,500 * y, 0));
			}
			else if (invertMap[y] == 3) {
				fps->setObjectivePos(Vector3(500 * i,500 * y, 0));
			}
			else if (invertMap[y] == 4) {
				Actor* cube = new DoorActor();
				cube->setScale(Vector3(500, 500, 500));
				cube->setPosition(Vector3(500 * i, 500 * y, 150));
				cube->setRotation(q);
			}
			else if (invertMap[y] == 5) {
				Actor* cube = new LockedDoorActor();
				cube->setScale(Vector3(500, 500, 500));
				cube->setPosition(Vector3(500 * i, 500 * y, 150));
				cube->setRotation(q);
			}
			else if (invertMap[y] == 6) {
				Actor* cube = new LockedDoorActor();
				LockedDoorActor* lockedDoor = dynamic_cast<LockedDoorActor*>(cube);
				lockedDoor->setKey(1);
				cube->setScale(Vector3(500, 500, 500));
				cube->setPosition(Vector3(500 * i, 500 * y, 150));
				cube->setRotation(q);
			}
			else if (invertMap[y] == 7) {
				Actor* cube = new KeyActor();
				KeyActor* key = dynamic_cast<KeyActor*>(cube);
				key->setKey(0);
				cube->setScale(Vector3(30, 30, 30));
				cube->setPosition(Vector3(500 * i, 500 * y, -50));
				cube->setRotation(q);
			}
			else if (invertMap[y] == 8) {
				Actor* cube = new KeyActor();
				KeyActor* key = dynamic_cast<KeyActor*>(cube);
				key->setKey(1);
				cube->setScale(Vector3(30, 30, 30));
				cube->setPosition(Vector3(500 * i, 500 * y, -50));
				cube->setRotation(q);
			}
			else if (invertMap[y] == 9) {
				auto mp = new MovingPlatformActor(false);
				mp->setStart(Vector3(500 * i, 500 * y, -101));
			}
			else if (invertMap[y] == 10) {
				auto mp = new MovingPlatformActor(true);
				mp->setStart(Vector3(500 * i, 500 * y, -101));
			}
			else if (invertMap[y] == 11) {

				Quaternion q2(Vector3::unitY, Maths::piOver2);
				q2 = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi * 2));

				auto p = new PlaneActor();
				p->getMesh()->setTextureIndex(2);
				p->setScale(Vector3(5.0f, 5.0f, 5.0f));
				p->setRotation(q2);
				p->setPosition(Vector3(500 * i - 250, 500 * y, -351.0f));

				auto p2 = new PlaneActor();
				p2->getMesh()->setTextureIndex(2);
				p2->setScale(Vector3(5.0f, 5.0f, 5.0f));
				p2->setRotation(q2);
				p2->setPosition(Vector3(500 * i + 250, 500 * y, -351.0f));

				q2 = Quaternion::concatenate(q2, Quaternion(Vector3::unitZ, Maths::piOver2));

				auto p3 = new PlaneActor();
				p3->getMesh()->setTextureIndex(2);
				p3->setScale(Vector3(5.0f, 5.0f, 5.0f));
				p3->setRotation(q2);
				p3->setPosition(Vector3(500 * i, 500 * y - 250, -351.0f));

				auto p4 = new PlaneActor();
				p4->getMesh()->setTextureIndex(2);
				p4->setScale(Vector3(5.0f, 5.0f, 5.0f));
				p4->setRotation(q2);
				p4->setPosition(Vector3(500 * i, 500 * y + 250, -351.0f));

				auto p5 = new PlaneActor();
				p5->getMesh()->setTextureIndex(3);
				p5->setScale(Vector3(5.0f, 5.0f, 5.0f));
				p5->setPosition(Vector3(500 * i, 500 * y, -601.0f));
			}
		}
	}

	for (int i = 0; i < map.size(); i++)
	{
		std::vector<int> invertMap = map[i];
		std::reverse(invertMap.begin(), invertMap.end());
		for (int y = 0; y < map[i].size(); y++)
		{
			if (invertMap[y] <= 8) {
				auto p = new PlaneActor();
				p->getMesh()->setTextureIndex(1);
				p->setScale(Vector3(5.0f, 5.0f, 5.0f));
				p->setPosition(Vector3(500 * i, 500 * y, -101.0f));
			}
		}
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
		bool isRunning = inputSystem.processEvent(event);
		if (!isRunning) state = GameState::Quit;
	}

	inputSystem.update();
	const InputState& input = inputSystem.getInputState();

	if (state == GameState::Gameplay)
	{
		// Escape: pause game
		if (input.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
		{
			new PauseScreen();
			return;
		}

		// Actor input
		isUpdatingActors = true;
		for (auto actor : actors)
		{
			actor->processInput(input);
		}
		isUpdatingActors = false;
		if (!UIStack.empty()) {
			// Update UI screens
			for (auto ui : UIStack)
			{
				if (ui->getState() == UIState::Active)
				{
					ui->processInput(input);
				}
			}
		}
	}
	else
	{
		if (!UIStack.empty())
		{
			UIStack.back()->processInput(input);
		}
	}

}

void Game::update(float dt)
{

	if (state == GameState::Gameplay)
	{
		// Update actors 
		isUpdatingActors = true;
		for (auto actor : actors)
		{
			actor->update(dt);
		}
		isUpdatingActors = false;

		// Move pending actors to actors
		for (auto pendingActor : pendingActors)
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

	// Update UI screens
	for (auto ui : UIStack)
	{
		if (ui->getState() == UIState::Active)
		{
			ui->update(dt);
		}
	}
	// Delete any UIScreens that are closed
	auto iter = UIStack.begin();
	while (iter != UIStack.end())
	{
		if ((*iter)->getState() == UIState::Closing)
		{
			delete* iter;
			iter = UIStack.erase(iter);
		}
		else
		{
			++iter;
		}
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
	while (state != GameState::Quit)
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
	Font::close();
	inputSystem.close();
	renderer.close();
	window.close();
	SDL_Quit();
}

void Game::setState(GameState stateP)
{
	state = stateP;
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

void Game::pushUI(UIScreen* screen)
{
	UIStack.emplace_back(screen);
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

void Game::addCube(CubeActor* cube)
{
	cubes.emplace_back(cube);
}

void Game::removeCube(CubeActor* cube)
{
	auto iter = std::find(begin(cubes), end(cubes), cube);
	cubes.erase(iter);
}

void Game::gameOver() {
	std::vector<std::vector<int>> map = Assets::getMap("BaseMap");

	for (int i = 0; i < map.size(); i++)
	{
		std::vector<int> invertMap = map[i];
		std::reverse(invertMap.begin(), invertMap.end());
		for (int y = 0; y < map[i].size(); y++)
		{
			if (invertMap[y] == 2) {
				fps->setPosition(Vector3(500 * i, 500 * y, 0));
			}
		}
	}
	HitPoints::instance().setHP(100);
}