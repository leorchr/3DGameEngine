#include "Assets.h"
#include "BallActor.h"
#include "BallMoveComponent.h"
#include "MeshComponent.h"

const float BallActor::lifetimeSpan = 3.0f;

BallActor::BallActor() : Actor(), ballMove(nullptr), timeLeft(lifetimeSpan)
{
	auto const mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Sphere"));
	ballMove = new BallMoveComponent(this);
	ballMove->setForwardSpeed(1000.0f);
}

void BallActor::updateActor(const float dt)
{
	Actor::updateActor(dt);

	timeLeft -= dt;
	if (timeLeft < 0.0f)
	{
		setState(ActorState::Dead);
	}
}

void BallActor::setPlayer(Actor* player) const
{
	ballMove->setPlayer(player);
}

void BallActor::hitTarget()
{
}