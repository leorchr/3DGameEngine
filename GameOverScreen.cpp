#include "Assets.h"
#include "GameOverScreen.h"
#include "Game.h"
#include "DialogBox.h"
#include <string>
#include <iostream>

GameOverScreen::GameOverScreen() : UIScreen()
{
	text = new Text("Game Over", Vector2(0.0f,0.0f));
	background = &Assets::getTexture("Blackbg");
	backgroundPosition = Vector2(0.0f, 0.0f);
}

GameOverScreen::~GameOverScreen()
{
	if (text)
	{
		text->getTexture()->unload();
		delete text;
	}
}

void GameOverScreen::draw(Shader& shader)
{
	UIScreen::draw(shader);
	drawTexture(shader, text->getTexture(), text->getPosition());
}

void GameOverScreen::processInput(const InputState& inputState)
{
	if (inputState.keyboard.getKeyState(SDL_SCANCODE_SPACE) == ButtonState::Pressed) {
		state = UIState::Closing;
	}
}
