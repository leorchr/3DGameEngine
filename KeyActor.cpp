#include "KeyActor.h"
#include "Assets.h"
#include "FPSActor.h"
#include "Game.h"
#include "Map.h"

KeyActor::KeyActor() :
	keyCode(0)
{
	Mesh* mesh = &Assets::getMesh("Mesh_Key");
	mc->setMesh(*mesh);
	mc->setTextureIndex(keyCode);
}

void KeyActor::setKey(int keyCode)
{
	this->keyCode = keyCode;
	mc->setTextureIndex(keyCode);
}

void KeyActor::onHit()
{
	Game::instance().getPlayer()->addKey(keyCode);
	setState(ActorState::Dead);
}
