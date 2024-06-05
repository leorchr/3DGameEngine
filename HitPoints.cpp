#include "Assets.h"
#include "HitPoints.h"
#include "Game.h"
#include "DialogBox.h"
#include <string>

HitPoints::HitPoints() : UIScreen(),
	hp(100)
{
	setTitle("LifePoints");
	titlePosition = Vector2(-890.0f, -450.0f);
	text = new Text("///", Vector2(-760.0f, -450.0f));
	background = &Assets::getTexture("ButtonYellow");
	backgroundPosition = Vector2(-760.0f, -448.0f);
}

HitPoints::~HitPoints()
{
}

void HitPoints::draw(Shader& shader)
{
	UIScreen::draw(shader);
	drawTexture(shader, text->getTexture(), text->getPosition());
}

void HitPoints::update(float dt) {
	UIScreen::update(dt);
	if (text != nullptr) {
		hp--;
		text->setContent(std::to_string(hp) + " / 100");
	}
}