#include "ShortcutsManager.h"
#include "Game.h"
#include "ImGUIWindow.h"
#include "InputSystem.h"
#include "SaveSystem.h"

void ShortcutsManager::processInput(const InputState& inputState) const
{
#ifdef _DEBUG
	switch(Game::instance().getMode())
	{
	case EngineMode::Game:
		if(inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Held &&
			inputState.keyboard.getKeyState(SDL_SCANCODE_P) == ButtonState::Pressed)
		{
			Game::instance().setMode(EngineMode::Editor);
		}
		break;
	case EngineMode::Editor:
		if (inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Held &&
			inputState.keyboard.getKeyState(SDL_SCANCODE_S) == ButtonState::Pressed)
		{
			SaveSystem::save();
		}
		if (inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Held &&
			inputState.keyboard.getKeyState(SDL_SCANCODE_LSHIFT) == ButtonState::Held &&
			inputState.keyboard.getKeyState(SDL_SCANCODE_S) == ButtonState::Pressed)
		{
			SaveSystem::saveAs();
		}
		if (inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Held &&
			inputState.keyboard.getKeyState(SDL_SCANCODE_L) == ButtonState::Pressed)
		{
			SaveSystem::loadFile();
		}
		if(inputState.keyboard.getKeyState(SDL_SCANCODE_LCTRL) == ButtonState::Held &&
			inputState.keyboard.getKeyState(SDL_SCANCODE_P) == ButtonState::Pressed)
		{
			Game::instance().setMode(EngineMode::Game);
		}
		if(inputState.keyboard.getKeyState(SDL_SCANCODE_DELETE) == ButtonState::Pressed)
		{
			Game::instance().getImGuiWindow()->deleteSelectedActor();
		}
		break;
	case EngineMode::None:
		break;
	}
#endif
}