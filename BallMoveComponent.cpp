#include "BallMoveComponent.h"
#include "Actor.h"
#include "Vector3.h"
#include "LineSegment.h"
#include "Collisions.h"
#include "PhysicsSystem.h"
#include "BallActor.h"
#include "Game.h"

#include <iostream>
using namespace std;

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
	setForwardSpeed(getForwardSpeed() * 0.995f);
	MoveComponent::update(dt);
	if (owner.getPosition().y > 110 || owner.getPosition().y < -110) dir = Vector3::unitX;

	const float segmentLength = 4.0f;
	Vector3 start = owner.getPosition();
	Vector3 dir = owner.getForward();
	Vector3 end = start + dir * segmentLength;
	LineSegment l(start, end);
	PhysicsSystem::CollisionInfo info;
	if (owner.getGame().getPhysicsSystem().segmentCast(l, info))
	{
		setForwardSpeed(0);
	}

}

void BallMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}
