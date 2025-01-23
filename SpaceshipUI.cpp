#include "SpaceshipUI.h"

#include "Assets.h"
#include "Game.h"
#include "DialogBox.h"
#include "SpaceshipActor.h"
#include "Texture.h"
#include <string>

SpaceshipUi::SpaceshipUi(SpaceshipActor& spaceshipActor) : UIScreen(), spaceshipActor(spaceshipActor)
{
	text = new Text("100", Vector2(0.0f,-100.0f));
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
	UIScreen::draw(shader);
	drawTexture(shader, text->getTexture(), text->getPosition());
}

void SpaceshipUi::updateText() const
{
	text->setContent(std::to_string(spaceshipActor.getCurrentLife()));
}