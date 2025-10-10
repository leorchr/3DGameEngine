#include "Game.h"
#include "EnemyActor.h"

#include "AkComponent.h"
#include "EnemyRocketActor.h"
#include "../Wwise/GeneratedSoundBanks/Wwise_IDs.h"

EnemyActor::EnemyActor() : MeshActor("Turret"), currentShootIntervals(baseShootingIntervals), currentLife(baseLife)
{
	akComponent = new AkComponent(this);
	setName("Enemy");
	game.addEnemy(this);
}

EnemyActor::~EnemyActor()
{
	game.removeEnemy(this);
}

void EnemyActor::updateActor(float dt)
{
	MeshActor::updateActor(dt);
	if(currentShootIntervals > 0) currentShootIntervals -= dt;
	else
	{
		shoot();
	}

	Vector3 start = position + Vector3(0.0f,20.0f,35.0f);
	Vector3 end = getGame().getPlayer()->getPosition();
		
	// Get direction vector
	Vector3 dir = end - start;
	dir.normalize();
}

const float EnemyActor::getRadius() const
{
	return radius;
}

void EnemyActor::shoot()
{
	currentShootIntervals = baseShootingIntervals;
		
	Vector3 start = position + Vector3(0.0f,20.0f,35.0f);
	Vector3 end = getGame().getPlayer()->getPosition() - Vector3(0.0f,0.0f, 10.0f);
		
	// Get direction vector
	Vector3 dir = end - start;
	if (dir.length() > 500.f) return;
	dir.normalize();
		
	// Spawn a ball
	EnemyRocketActor* ball = new EnemyRocketActor();
	ball->setPosition(start + dir * 20.0f);
	ball->setScale(Vector3(2.0f,2.0f,2.0f));
		
	// Rotate the ball to face new direction
	ball->rotateToNewForward(dir);

	if (akComponent != nullptr)
	{
		akComponent->PostEvent(AK::EVENTS::PLAY_SHOT);
	}
}

void EnemyActor::onHit(int damages)
{
	currentLife -= damages;
	if(currentLife <= 0)
	{
		setState(ActorState::Dead);
	}
}