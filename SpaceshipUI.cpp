#include "SpaceshipUI.h"
#include "Game.h"
#include "SpaceshipActor.h"
#include "Text.h"
#include "Texture.h"
#include <string>

SpaceshipUi::SpaceshipUi(SpaceshipActor& spaceshipActor) : UIScreen(), spaceshipActor(spaceshipActor)
{
	text = new Text("100", Vector2(0.0f,-400.0f*WINDOW_WIDTH/1920));
}

SpaceshipUi::~SpaceshipUi()
{
	if (text)
	{
		text->getTexture()->unload();
		delete text;
	}
}

void SpaceshipUi::draw(Shader& shader)
{
	if(state != UIState::Active) return;
	UIScreen::draw(shader);
	drawTexture(shader, text->getTexture(), text->getPosition());
}

void SpaceshipUi::updateText() const
{
	text->setContent(std::to_string(spaceshipActor.getCurrentLife()));
}