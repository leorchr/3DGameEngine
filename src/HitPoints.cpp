#include "Assets.h"
#include "HitPoints.h"
#include "DialogBox.h"
#include "GameOverScreen.h"
#include <string>

HitPoints::HitPoints() : UIScreen(),
	hp(100),
	onlyOnce(false)
{
	setTitle("LifePoints");
	titlePosition = Vector2(-890.0f, -450.0f);
	text = new Text("100 / 100", Vector2(-760.0f, -450.0f));
	background = &Assets::getTexture("ButtonYellow");
	backgroundPosition = Vector2(-760.0f, -448.0f);
}

HitPoints::~HitPoints()
{
	if (text)
	{
		text->getTexture()->unload();
		delete text;
	}
}

void HitPoints::draw(Shader& shader)
{
	UIScreen::draw(shader);
	drawTexture(shader, text->getTexture(), text->getPosition());
}

void HitPoints::update(float dt) {
	UIScreen::update(dt);
}

void HitPoints::setHP(int hpP)
{
	if (text != nullptr) {
		hp = hpP;
		text->setContent(std::to_string(hp) + " / 100");
		if (hp <= 0) {
			new GameOverScreen();
		}
	}
}