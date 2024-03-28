#include "BallMoveComponent.h"
#include "Actor.h"
#include "Vector3.h"
#include "LineSegment.h"
#include "Collisions.h"
#include "PhysicsSystem.h"
#include "BallActor.h"
#include "TargetActor.h"
#include "Game.h"

BallMoveComponent::BallMoveComponent(Actor* ownerP) : MoveComponent(ownerP), player(nullptr)
{
	dir = owner.getForward();
}

void BallMoveComponent::setPlayer(Actor* playerP)
{
	player = playerP;
}

void BallMoveComponent::update(float dt)
{
	owner.rotateToNewForward(dir);
	// Base class update moves based on forward speed
	MoveComponent::update(dt);
}

void BallMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}
