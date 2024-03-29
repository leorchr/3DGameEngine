#include "PinMoveComponent.h"
#include "Actor.h"
#include "Vector3.h"
#include "Vector2.h"
#include "LineSegment.h"
#include "Collisions.h"
#include "PhysicsSystem.h"
#include "BallActor.h"
#include "Game.h"
#include "PinActor.h"

PinMoveComponent::PinMoveComponent(Actor* ownerP) : MoveComponent(ownerP), player(nullptr)
{
	dir = owner.getForward();
}

void PinMoveComponent::setPlayer(Actor* playerP)
{
	player = playerP;
}

void PinMoveComponent::update(float dt)
{
	owner.rotateToNewForward(dir);
	setForwardSpeed(getForwardSpeed() * 0.995f);
	setStrafeSpeed(getStrafeSpeed() * 0.995f);
	MoveComponent::update(dt);

	if (owner.getPosition().y > 110 || owner.getPosition().y < -110) dir = Vector3::unitX;

	PhysicsSystem::CollisionInfo info;

	PinActor* pinActor = static_cast<PinActor*>(&owner);

	if (owner.getGame().getPhysicsSystem().boxCast(*pinActor->getBoxComponent(), info))
	{
		PinActor* pinActor = static_cast<PinActor*>(info.actor);
		if (pinActor)
		{
			if (!pinActor->getOnlyOnce()) {
				Vector3 hitDir = pinActor->getPosition() - owner.getPosition();
				hitDir.normalize();
				hitDir.z = 0;

				pinActor->getMoveComponent()->setForwardSpeed(getForwardSpeed()* hitDir.x * 0.7f);
				pinActor->getMoveComponent()->setStrafeSpeed(getStrafeSpeed()* hitDir.y * 0.7f);
				setForwardSpeed(getForwardSpeed() * 0.65f);
				setStrafeSpeed(getStrafeSpeed() * 0.65f);
				pinActor->onHit();
			}
		}
	}

}

void PinMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}
