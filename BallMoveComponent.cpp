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
	if (owner.getPosition().y > 110 || owner.getPosition().y < -110) {
		if(owner.getPosition().z == -85.0f) owner.setPosition(Vector3(owner.getPosition().x, owner.getPosition().y, owner.getPosition().z - 10));
		setVelocity(Vector3::unitX * 50);
	}

	PhysicsSystem::CollisionInfo info;

	BallActor* ballActor = static_cast<BallActor*>(&owner);

	if (owner.getGame().getPhysicsSystem().boxCast(*ballActor->getBoxComponent(), info))
	{
		PinActor* pinActor = static_cast<PinActor*>(info.actor);
		if (pinActor)
		{
			Vector3 hitDir = pinActor->getPosition() - owner.getPosition();
			hitDir.z = 0;
			hitDir.normalize();
			Vector3 f = hitDir * getVelocity().length();
			ballActor->getBallMoveComponent()->setVelocity(ballActor->getBallMoveComponent()->getVelocity()/2);
			pinActor->getMoveComponent()->addForce(f/2.0f);
			pinActor->onHit();
		}
	}
	MoveComponent::update(dt);
}

void BallMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}