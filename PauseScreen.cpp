#include "PauseScreen.h"
#include "Game.h"
PauseScreen::PauseScreen() : UIScreen()
{
	Game::instance().setState(GameState::Pause);
	setTitle("PauseTitle");
}
PauseScreen::~PauseScreen()
{
	Game::instance().setState(GameState::Gameplay);
}
void PauseScreen::processInput(const InputState& inputState)
{
	UIScreen::processInput(inputState);
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_ESCAPE) ==
		ButtonState::Released)
	{
		close();
	}
}