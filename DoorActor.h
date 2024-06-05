#pragma once
#include "CubeActor.h"

class DoorActor : public CubeActor
{
public:
	DoorActor();
	void updateActor(float dt) override;
	void onHit();

private:
	bool isHit;
	float timeLeft;
	static const float timeUp;
};