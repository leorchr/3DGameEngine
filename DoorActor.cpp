#include "DoorActor.h"

const float DoorActor::timeUp = 3.0f;

DoorActor::DoorActor() :
	isHit(false),
	timeLeft(timeUp)
{
	mc->setTextureIndex(1);
}

void DoorActor::updateActor(float dt)
{
	if(isHit) {
		timeLeft -= dt;
		if (timeLeft <= 0) {
			Vector3 position = getPosition();
			position.z -= 500;
			setPosition(position);
			isHit = false;
			timeLeft = timeUp;
		}
	}
}

void DoorActor::onHit() {
	if (!isHit) {
		Vector3 position = getPosition();
		position.z += 500;
		setPosition(position);
		isHit = true;
	}
}