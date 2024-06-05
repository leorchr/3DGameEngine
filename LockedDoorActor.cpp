#include "LockedDoorActor.h"
#include "Game.h"
#include <iostream>

const float LockedDoorActor::timeUp = 3.0f;

LockedDoorActor::LockedDoorActor() :
	isHit(false),
	timeLeft(timeUp)
{
	mc->setTextureIndex(2);
}

void LockedDoorActor::updateActor(float dt)
{
	if (isHit) {
		timeLeft -= dt;
		std::cout << timeLeft << std::endl;
		if (timeLeft <= 0) {
			Vector3 position = getPosition();
			position.z -= 500;
			setPosition(position);
			isHit = false;
			timeLeft = timeUp;
		}
	}
}

void LockedDoorActor::onHit() {
	if (!isHit) {
		Vector3 position = getPosition();
		position.z += 500;
		setPosition(position);
		isHit = true;
	}
}