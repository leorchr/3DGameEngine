#pragma once
#include "Actor.h"
#include "CubeActor.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "PlaneActor.h"
#include "RendererOGL.h"
#include "SpriteComponent.h"
#include "Window.h"
#include <vector>

using std::vector;

enum class GameState
{
	Gameplay, Pause, Quit
};

class Game
{
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

private:
	Game() : state(GameState::Gameplay), isUpdatingActors(false), player(nullptr) {}

public:
	bool initialize();
	void load();
	void loop();
	void unload();
	void close();

	GameState getState() const { return state; }
	void setState(GameState stateP);

	void addActor(Actor* actor);
	void removeActor(Actor* actor);
	RendererOGL& getRenderer() { return renderer; }
	PhysicsSystem& getPhysicsSystem() { return physicsSystem; }
	InputSystem& getInputSystem() { return inputSystem; }
	const vector<class UIScreen*>& getUIStack() { return UIStack; }
	void pushUI(class UIScreen* screen);

	// Game-specific
	void addPlane(class PlaneActor* plane);
	void removePlane(class PlaneActor* plane);
	vector<PlaneActor*>& getPlanes() { return planes; }

	void addCube(class CubeActor* cube);
	void removeCube(class CubeActor* cube);
	vector<CubeActor*>& getCubes() { return cubes; }
	class TPActor* getPlayer() { return player; }

private:
	void processInput();
	void update(float dt);
	void render();

	GameState state;
	Window window;
	RendererOGL renderer;
	InputSystem inputSystem;
	PhysicsSystem physicsSystem;
	vector<class UIScreen*> UIStack;

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

	// Game specific
	class TPActor* player;
	vector<PlaneActor*> planes;
	vector<CubeActor*> cubes;
};