#include "BallActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "BallMoveComponent.h"
#include "Game.h"

BallActor::BallActor() : Actor(), lifetimeSpan(5.0f), ballMove(nullptr)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Sphere"));

	ballMove = new BallMoveComponent(this);
	ballMove->setForwardSpeed(750.0f);

	boxComponent = new BoxComponent(this);
	boxComponent->setObjectBox(Assets::getMesh("Mesh_Sphere").getBox());
}

void BallActor::updateActor(float dt)
{
	Actor::updateActor(dt);

	lifetimeSpan -= dt;
	if (lifetimeSpan < 0.0f)
	{
		setState(ActorState::Dead);
		getGame().getPlayer()->currentState++;
		if (getGame().getScore() == 10 || getGame().getPlayer()->shotNumber == 2) getGame().endGame();
		//show score

	}
}

void BallActor::setPlayer(Actor* player)
{
	ballMove->setPlayer(player);
}
