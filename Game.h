#pragma once
#include "Actor.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "RendererOGL.h"
#include "Window.h"
#include <vector>
#include <string>

enum class GameState
{
	Running, Pause, Quit
};

enum class EngineMode
{
	Editor, Game, None
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
	Game() : state(GameState::Running), mode(EngineMode::None), isUpdatingActors(false), player(nullptr) {}

public:
	bool initialize();
	void load();
	void loop();
	void unload();
	void close();

	GameState getState() const { return state; }
	void setState(GameState stateP);
	
	#ifdef _DEBUG
	EngineMode getMode() const { return mode; }
	void setMode(EngineMode mode);
	#endif

	void addActor(Actor* actor);
	void removeActor(Actor* actor);
	void setActorNewName(class Actor* actor);
	RendererOGL& getRenderer() { return renderer; }
	Window& getWindow() { return window; }
	PhysicsSystem& getPhysicsSystem() { return physicsSystem; }
	InputSystem& getInputSystem() { return inputSystem; }
	const std::vector<class UIScreen*>& getUIStack() { return UIStack; }
	void pushUI(class UIScreen* screen);

	// Game-specific
	void addPlane(class PlaneActor* plane);
	void removePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& getPlanes() { return planes; }

	void addCube(class CubeActor* cube);
	void removeCube(class CubeActor* cube);
	std::vector<class CubeActor*>& getCubes() { return cubes; }
	class Actor* getPlayer() { return player; }
	class std::vector<Actor*>& getActors() { return actors; }

#ifdef _DEBUG
	class ImGUIWindow* getImGuiWindow(){ return imGuiWindow; }
#endif
	
private:
	void processInput();
	void update(float dt);
	void render();

	EngineMode mode;
	GameState state;
	Window window;
	RendererOGL renderer;
	InputSystem inputSystem;
	PhysicsSystem physicsSystem;
	std::vector<class UIScreen*> UIStack;

	bool isUpdatingActors;
	std::vector<Actor*> actors;
	std::vector<Actor*> pendingActors;
	std::vector<std::string> actorNames;

#ifdef _DEBUG
	class ImGUIWindow* imGuiWindow;
#endif
	// Game specific
	class Actor* player;
	
	std::vector<class PlaneActor*> planes;
	std::vector<class CubeActor*> cubes;
};