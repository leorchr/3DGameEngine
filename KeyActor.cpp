#include "KeyActor.h"
#include "Vector3.h"
#include "Mesh.h"
#include "Assets.h"

KeyActor::KeyActor() :
	keyCode(0)
{
	Mesh* mesh = &Assets::getMesh("Mesh_Key");
	mc->setTextureIndex(keyCode);
}

KeyActor::~KeyActor()
{
}

void KeyActor::setKey(int keyCode)
{
	this->keyCode = keyCode;
	mc->setTextureIndex(keyCode);
}

void KeyActor::onHit()
{
}
