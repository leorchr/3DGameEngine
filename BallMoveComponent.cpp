#include "BallMoveComponent.h"
#include "Actor.h"
#include "Vector3.h"
#include "LineSegment.h"
#include "Collisions.h"
#include "PhysicsSystem.h"
#include "BallActor.h"
#include "Game.h"
#include "PinActor.h"

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

	PhysicsSystem::CollisionInfo info;

	BallActor* ballActor = static_cast<BallActor*>(&owner);

	if (owner.getGame().getPhysicsSystem().boxCast(*ballActor->getBoxComponent(), info))
	{
		PinActor* pinActor = static_cast<PinActor*>(info.actor);
		if (pinActor)
		{
			if (!pinActor->getOnlyOnce()) {
				Vector3 hitDir = pinActor->getPosition() - owner.getPosition();
				hitDir.normalize();
				hitDir.z = 0;
				pinActor->getMoveComponent()->setForwardSpeed(getForwardSpeed()*hitDir.x * 0.7f);
				pinActor->getMoveComponent()->setStrafeSpeed(getForwardSpeed()* hitDir.y * 0.7f);
				setForwardSpeed(getForwardSpeed() * 0.65f);
				pinActor->onHit();
			}
		}
	}

}

void BallMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}
