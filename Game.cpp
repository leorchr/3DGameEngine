#include "Game.h"
#include "Assets.h"
#include "Font.h"
#include "PauseScreen.h"
#include "PlaneActor.h"
#include "Texture.h"
#include "Timer.h"
#include "TPActor.h"
#include <vector>

bool Game::initialize()
{
	const bool isWindowInit = window.initialize();
	const bool isRendererInit = renderer.initialize(window);
	const bool isInputInit = inputSystem.initialize();
	const bool isFontInit = Font::initialize();

	return isWindowInit && isRendererInit && isInputInit && isFontInit; // Return bool && bool && bool ...to detect error
}

void Game::load()
{
	inputSystem.setMouseRelativeMode(true);

	Assets::loadShader("Ressources/Shaders/Sprite.vert", "Ressources/Shaders/Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("Ressources/Shaders/Phong.vert", "Ressources/Shaders/Phong.frag", "", "", "", "Phong");
	Assets::loadShader("Ressources/Shaders/BasicMesh.vert", "Ressources/Shaders/BasicMesh.frag", "", "", "", "BasicMesh");
	Assets::loadShader("Ressources/Shaders/Mesh.vert", "Ressources/Shaders/Mesh.frag", "", "", "", "Mesh");
	
	Assets::loadTexture(renderer, "Ressources/Textures/ButtonYellow.png", "ButtonYellow");
	Assets::loadTexture(renderer, "Ressources/Textures/ButtonBlue.png", "ButtonBlue");
	Assets::loadTexture(renderer, "Ressources/Textures/DialogBG.png", "DialogBG");
	
	Assets::loadMesh("Ressources/Meshes/plane.fbx", "Mesh_Plane");
	Assets::loadMesh("Ressources/Meshes/cube.fbx", "Mesh_Cube");
	Assets::loadMesh("Ressources/Meshes/moto.fbx", "Mesh_Moto");

	Assets::loadFont("Ressources/Fonts/Carlito-Regular.ttf", "Carlito");
	Assets::loadText("Ressources/Localization/English.gptext");
	
	player = new TPActor();
	player->setPosition(Vector3(0.0f,0.0f,1.0f));

	for(int i = 0; i < 5; i++)
	{
		for(int y = 0; y < 5; y++)
		{
			auto plane = new PlaneActor();
			plane->setScale(Vector3(50,50,1));
			Vector3 pos = Vector3(plane->getPosition().x + i * 100, plane-> getPosition().y + y * 100, 0.0f);
			plane->setPosition(pos);
		}
	}

	// Setup lights
	renderer.setAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
	DirectionalLight& dir = renderer.getDirectionalLight();
	dir.direction = Vector3(0.5f, 0.2f, -0.7f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

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