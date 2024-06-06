#pragma once
#include "CubeActor.h"

class LockedDoorActor : public CubeActor
{
public:
	LockedDoorActor();
	void updateActor(float dt) override;
	void onHit();
	void setKey(int key);

private:
	bool isHit;
	int key;
	float timeLeft;
	static const float timeUp;
};