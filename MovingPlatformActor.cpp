#include "MovingPlatformActor.h"
#include "MeshComponent.h"
#include "Game.h"

MovingPlatformActor::MovingPlatformActor(bool isYPlatform) :
	toEnd(true),
	isYPlatform(isYPlatform)
{
	mc->setTextureIndex(2);
	setScale(Vector3(5.0f, 5.0f, 5.0f));
}

MovingPlatformActor::~MovingPlatformActor(){}

void MovingPlatformActor::updateActor(float dt)
{
	Actor::updateActor(dt);

	if(isYPlatform)
	{
		updateYPlatform(dt);
	}
	else
	{
		updateXPlatform(dt);
	}
}

void MovingPlatformActor::updateXPlatform(float dt) {
	if (getPosition().x <= end.x && toEnd)
	{
		Vector3 position = getPosition();
		position.x += 100.0f * dt;
		setPosition(position);
	}
	else {
		toEnd = false;
	}

	if (getPosition().x >= start.x && !toEnd) {
		Vector3 position = getPosition();
		position.x -= 100.0f * dt;
		setPosition(position);
	}
	else {
		toEnd = true;
	}

	if (Game::instance().getPlayer()->getPosition().x > getPosition().x - 250.0f && Game::instance().getPlayer()->getPosition().x < getPosition().x + 250.0f &&
		Game::instance().getPlayer()->getPosition().y > getPosition().y - 250.0f && Game::instance().getPlayer()->getPosition().y < getPosition().y + 250.0f) {
		if (toEnd) {
			Vector3 position = Game::instance().getPlayer()->getPosition();
			position.x += 100.0f * dt;
			Game::instance().getPlayer()->setPosition(position);
		}
		else {
			Vector3 position = Game::instance().getPlayer()->getPosition();
			position.x -= 100.0f * dt;
			Game::instance().getPlayer()->setPosition(position);
		}
	}
}

void MovingPlatformActor::updateYPlatform(float dt) {
	if (getPosition().y <= end.y && toEnd)
	{
		Vector3 position = getPosition();
		position.y += 100.0f * dt;
		setPosition(position);
	}
	else {
		toEnd = false;
	}

	if (getPosition().y >= start.y && !toEnd) {
		Vector3 position = getPosition();
		position.y -= 100.0f * dt;
		setPosition(position);
	}
	else {
		toEnd = true;
	}

	if (Game::instance().getPlayer()->getPosition().x > getPosition().x - 250.0f && Game::instance().getPlayer()->getPosition().x < getPosition().x + 250.0f &&
		Game::instance().getPlayer()->getPosition().y > getPosition().y - 250.0f && Game::instance().getPlayer()->getPosition().y < getPosition().y + 250.0f) {
		if (toEnd) {
			Vector3 position = Game::instance().getPlayer()->getPosition();
			position.y += 100.0f * dt;
			Game::instance().getPlayer()->setPosition(position);
		}
		else {
			Vector3 position = Game::instance().getPlayer()->getPosition();
			position.y -= 100.0f * dt;
			Game::instance().getPlayer()->setPosition(position);
		}
	}
}

void MovingPlatformActor::setStart(Vector3 startP)
{
	start = startP;
	if(isYPlatform) end = Vector3(start.x, start.y + 500.0f, start.z);
	else end = Vector3(start.x + 500.0f, start.y, start.z);
	setPosition(start);
}
