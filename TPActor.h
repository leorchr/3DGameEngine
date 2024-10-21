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
};