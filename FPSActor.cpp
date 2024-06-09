#include "FPSActor.h"
#include "MoveComponent.h"
#include "Game.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "BallActor.h"
#include "BoxComponent.h"
#include "KeyActor.h"
#include "Collisions.h"
#include "HitPoints.h"
#include <iostream>
#include <vector>

FPSActor::FPSActor() :
	Actor(),
	moveComponent(nullptr),
	meshComponent(nullptr),
	cameraComponent(nullptr),
	boxComponent(nullptr)
{
	keys = {{0, false},{1, false}};
	moveComponent = new MoveComponent(this);
	moveComponent->setGravitySpeed(-100.0f);
	moveComponent->setEnableGravity(true);
	cameraComponent = new FPSCameraComponent(this);

	FPSModel = new Actor();
	FPSModel->setScale(Vector3(0.75f,0.75f,0.75f));
	meshComponent = new MeshComponent(FPSModel);
	meshComponent->setMesh(Assets::getMesh("Mesh_Rifle"));

	boxComponent = new BoxComponent(this);
	AABB collision(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
	boxComponent->setObjectBox(collision);
	boxComponent->setShouldRotate(false);
	objectivePos = Vector3(Maths::infinity, 0, 0);
}

void FPSActor::updateActor(float dt)
{
	Actor::updateActor(dt);

	// Update position and rotation of model relatively to position
	Vector3 modelPosition = getPosition();
	modelPosition += getForward() * MODEL_OFFSET.x;
	modelPosition += getRight() * MODEL_OFFSET.y;
	modelPosition.z += MODEL_OFFSET.z;
	FPSModel->setPosition(modelPosition);
	Quaternion q = getRotation();
	q = Quaternion::concatenate(q, Quaternion(getRight(), cameraComponent->getPitch()));
	FPSModel->setRotation(q);

	fixCollisions();

	float sqDist = Maths::pow(objectivePos.x - getPosition().x) + Maths::pow(objectivePos.y - getPosition().y);
	if (sqDist < 30000) {
		Game::instance().setState(GameState::Quit);
	}


	if (getPosition().z < -510.0f) {
		HitPoints::instance().setHP(HitPoints::instance().getHP() - 50);

		std::vector<std::vector<int>> map = Assets::getMap("BaseMap");

		for (int i = 0; i < map.size(); i++)
		{
			std::vector<int> invertMap = map[i];
			std::reverse(invertMap.begin(), invertMap.end());
			for (int y = 0; y < map[i].size(); y++)
			{
				if (invertMap[y] == 2) {
					setPosition(Vector3(500 * i, 500 * y, 0));
				}
			}
		}
	}
}

void FPSActor::actorInput(const InputState& inputState)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		forwardSpeed += 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		forwardSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		strafeSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed += 400.0f;
	}
	moveComponent->setForwardSpeed(forwardSpeed);
	moveComponent->setStrafeSpeed(strafeSpeed);
	// Mouse mouvement
	Vector2 mousePosition = inputState.mouse.getPosition();
	float x = mousePosition.x;
	float y = mousePosition.y;
	const int maxMouseSpeed = 500;
	const float maxAngularSpeed = Maths::pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		angularSpeed = x / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	moveComponent->setAngularSpeed(angularSpeed);
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	cameraComponent->setPitchSpeed(pitchSpeed);

	// Shoot
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed)
	{
		shoot();
	}
}

void FPSActor::shoot()
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = getGame().getRenderer().unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = getGame().getRenderer().unproject(screenPoint);
	// Get direction vector
	Vector3 dir = end - start;
	dir.normalize();
	// Spawn a ball
	BallActor* ball = new BallActor();
	ball->setPlayer(this);
	ball->setPosition(start + dir * 20.0f);
	// Rotate the ball to face new direction
	ball->rotateToNewForward(dir);
}


void FPSActor::setVisible(bool isVisible)
{
	meshComponent->setVisible(isVisible);
}

void FPSActor::addKey(int key)
{
	keys[key] = true;
}

bool FPSActor::haveKey(int key)
{
	return keys[key];
}

void FPSActor::fixCollisions()
{
	// Need to recompute world transform to update world box
	computeWorldTransform();

	const AABB& playerBox = boxComponent->getWorldBox();
	Vector3 pos = getPosition();

	auto& planes = getGame().getPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->getBox()->getWorldBox();
		if (Collisions::intersect(playerBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.max.x - playerBox.min.x;
			float dx2 = planeBox.min.x - playerBox.max.x;
			float dy1 = planeBox.max.y - playerBox.min.y;
			float dy2 = planeBox.min.y - playerBox.max.y;
			float dz1 = planeBox.max.z - playerBox.min.z;
			float dz2 = planeBox.min.z - playerBox.max.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Maths::abs(dx1) < Maths::abs(dx2) ? dx1 : dx2;
			// Ditto for dy
			float dy = Maths::abs(dy1) < Maths::abs(dy2) ? dy1 : dy2;
			// Ditto for dz
			float dz = Maths::abs(dz1) < Maths::abs(dz2) ? dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Maths::abs(dx) <= Maths::abs(dy) && Maths::abs(dx) <= Maths::abs(dz))
			{
				pos.x += dx;
			}
			else if (Maths::abs(dy) <= Maths::abs(dx) && Maths::abs(dy) <= Maths::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			setPosition(pos);
			boxComponent->onUpdateWorldTransform();
		}
	}

	auto& cubes = getGame().getCubes();
	for (auto ca : cubes)
	{
		// Do we collide with this PlaneActor?
		const AABB& cubeBox = ca->getBox()->getWorldBox();
		if (Collisions::intersect(playerBox, cubeBox))
		{
			// Calculate all our differences
			float dx1 = cubeBox.max.x - playerBox.min.x;
			float dx2 = cubeBox.min.x - playerBox.max.x;
			float dy1 = cubeBox.max.y - playerBox.min.y;
			float dy2 = cubeBox.min.y - playerBox.max.y;
			float dz1 = cubeBox.max.z - playerBox.min.z;
			float dz2 = cubeBox.min.z - playerBox.max.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Maths::abs(dx1) < Maths::abs(dx2) ? dx1 : dx2;
			// Ditto for dy
			float dy = Maths::abs(dy1) < Maths::abs(dy2) ? dy1 : dy2;
			// Ditto for dz
			float dz = Maths::abs(dz1) < Maths::abs(dz2) ? dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Maths::abs(dx) <= Maths::abs(dy) && Maths::abs(dx) <= Maths::abs(dz))
			{
				pos.x += dx;
			}
			else if (Maths::abs(dy) <= Maths::abs(dx) && Maths::abs(dy) <= Maths::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			setPosition(pos);
			boxComponent->onUpdateWorldTransform();

			KeyActor* key = dynamic_cast<KeyActor*>(ca);
			if (key)
			{
				key->onHit();
			}
		}
	}
}

void FPSActor::setObjectivePos(Vector3 objectivePos)
{
	this->objectivePos = objectivePos;
}
