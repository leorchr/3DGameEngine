#include "LockedDoorActor.h"
#include "Game.h"
#include "FPSActor.h"
#include "Mesh.h"
#include "Assets.h"

const float LockedDoorActor::timeUp = 3.0f;

LockedDoorActor::LockedDoorActor() :
	isHit(false),
	timeLeft(timeUp),
	key(0)
{
	Mesh* mesh = &Assets::getMesh("Mesh_LockedDoor");
	mc->setMesh(*mesh);
	mc->setTextureIndex(key);
}

void LockedDoorActor::updateActor(float dt)
{
	if (isHit) {
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

void LockedDoorActor::onHit() {
	if (Game::instance().getPlayer()->haveKey(key)) {
		if (!isHit) {
			Vector3 position = getPosition();
			position.z += 500;
			setPosition(position);
			isHit = true;
		}
	}
}

void LockedDoorActor::setKey(int key)
{
	this->key = key;
	mc->setTextureIndex(key);
}
