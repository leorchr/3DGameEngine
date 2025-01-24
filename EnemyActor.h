#pragma once
#include "MeshActor.h"

class EnemyActor : public MeshActor
{
public:
	EnemyActor();
	virtual ~EnemyActor() override;
	EnemyActor(const Actor&) = delete;
	EnemyActor& operator=(const EnemyActor&) = delete;

	void updateActor(float dt) override;
	const float getRadius() const;
	void shoot();
	void onHit(int damages);

private:
	const float baseShootingIntervals = 2.0f;
	float currentShootIntervals;

	const int baseLife = 100;
	int currentLife;

	const float radius = 50;
};
