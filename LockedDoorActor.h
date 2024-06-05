#pragma once
#include "CubeActor.h"

class LockedDoorActor : public CubeActor
{
public:
	LockedDoorActor();
	void updateActor(float dt) override;
	void onHit();

private:
	bool isHit;
	float timeLeft;
	static const float timeUp;
};