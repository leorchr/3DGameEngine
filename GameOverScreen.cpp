#include "Assets.h"
#include "GameOverScreen.h"
#include "Game.h"
#include "DialogBox.h"
#include "SaveSystem.h"
#include "SpaceshipActor.h"
#include "Texture.h"
#include <string>

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
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed) {
		SaveSystem::reloadCurrentMap();
		SpaceshipActor* player = new SpaceshipActor();
		player->setPosition(Vector3(5.0f,5.0f,30.0f));
		Game::instance().setPlayer(player);
		close();
	}
}
