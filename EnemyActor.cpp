#include "EnemyActor.h"
#include "Game.h"
#include "EnemyRocketActor.h"

EnemyActor::EnemyActor() : MeshActor("Turret"), currentShootIntervals(baseShootingIntervals)
{
	setName("Enemy");
}

EnemyActor::~EnemyActor() {}

void EnemyActor::updateActor(float dt)
{
	MeshActor::updateActor(dt);
	if(currentShootIntervals > 0) currentShootIntervals -= dt;
	else
	{
		currentShootIntervals = baseShootingIntervals;
		
		Vector3 start = position + Vector3(0.0f,20.0f,35.0f);
		Vector3 end = getGame().getPlayer()->getPosition();
		// Get direction vector
		Vector3 dir = end - start;
		dir.normalize();
		// Spawn a ball
		EnemyRocketActor* ball = new EnemyRocketActor();
		ball->setPosition(start + dir * 20.0f);
		ball->setScale(Vector3(2.0f,2.0f,2.0f));
		// Rotate the ball to face new direction
		ball->rotateToNewForward(dir);
	}
}