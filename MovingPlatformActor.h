#pragma once
#include "PlaneActor.h"
#include "Vector3.h"

class MovingPlatformActor : public PlaneActor
{
public:
	MovingPlatformActor(bool isYPlatform);
	~MovingPlatformActor();
	void updateActor(float dt) override;
	void updateXPlatform(float dt);
	void updateYPlatform(float dt);
	void setStart(Vector3 startP);

private:
	Vector3 start;
	Vector3 end;
	bool toEnd;
	bool isYPlatform;
};