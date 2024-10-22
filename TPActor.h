#include "Actor.h"
#pragma once

class TPActor : public Actor
{
public:
	TPActor();
	~TPActor() override;
	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;

private:
	class MoveComponent* moveComponent;
	class MeshComponent* meshComponent;
	class TPSCameraComponent* cameraComponent;


	const float rotationSpeed = 2.0f;
	const float baseSpeed = 400.0f;
	float lerpValue;
	Vector3 cameraForwardRaw;
	Quaternion baseRotation;
	bool cameraLock;
};