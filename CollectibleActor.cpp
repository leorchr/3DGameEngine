#include "CollectibleActor.h"
#include "Assets.h"
#include "Game.h"
#include "MeshComponent.h"
#include "SpaceshipActor.h"

CollectibleActor::CollectibleActor() :
	MeshActor()
{
	meshComponent->setMesh(Assets::getMesh("Sphere"));
	setName("Collectible");
	game.addCollectible(this);
}

CollectibleActor::~CollectibleActor()
{
	game.removeCollectible(this);
}

void CollectibleActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

const float CollectibleActor::getRadius() const
{
	return scale.x * radius;
}

void CollectibleActor::onHit()
{
	auto player = dynamic_cast<SpaceshipActor*>(game.getPlayer());
	player->onCollect(healAmount);
	setState(ActorState::Dead);
}