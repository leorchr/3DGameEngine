#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "Assets.h"


class ArrowActor : public Actor
{
public:
	ArrowActor(float rotationSpeedP, float scaleSpeedP);

	void updateActor(float dt) override;
	void setVisible(bool isVisible);
	void setCurrentState(int currentStateP);
	int getCurrentState() { return currentState; }
	float getMaxScaleY() { return maxScaleY; }
	float getMinScaleY() { return minScaleY; }

private:
	MeshComponent* mc;
	float rotationSpeed;
	float scaleSpeed;
	int currentState;
	Vector3 baseScale;

	Quaternion newRotation;
	Quaternion increment;
	float angle;
	float yaw;
	float scaleY;
	float maxScaleY;
	float minScaleY;
};

