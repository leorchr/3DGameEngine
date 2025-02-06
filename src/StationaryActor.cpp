#include "StationaryActor.h"
#include "StationaryCameraComponent.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Assets.h"

StationaryActor::StationaryActor(float pitchP, float yawP) :
	Actor(),
	cameraComponent(nullptr)
{
	cameraComponent = new StationaryCameraComponent(this);
	cameraComponent->setPitch(pitchP);
	cameraComponent->setYaw(yawP);
}

void StationaryActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void StationaryActor::setCameraOrientation()
{
	cameraComponent->recomputeRotation();
}