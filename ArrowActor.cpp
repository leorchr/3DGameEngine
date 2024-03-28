#include "ArrowActor.h"
#pragma once
#include <iostream>
using namespace std;

ArrowActor::ArrowActor(float speedP) :
	speed(speedP)
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));
	setScale(Vector3(50.0f, 5.0f, 5.0f));
	setRotation(Quaternion::identity);
	setPosition(Vector3(80.0f, 0.0f, -60.0f));
}

void ArrowActor::updateActor(float dt)
{
	Quaternion newRotation = getRotation();
	float angle = speed * dt;
	Quaternion increment(Vector3::unitZ, angle);
	newRotation = Quaternion::concatenate(newRotation, increment);
	float yaw = Maths::asin(2 * newRotation.x * newRotation.y + 2 * newRotation.z * newRotation.w);
	if (Maths::toDegrees(yaw) > 45 && speed > 0) speed = -Maths::abs(speed);
	if (Maths::toDegrees(yaw) < -45 && speed < 0) speed = Maths::abs(speed);
	setRotation(newRotation);
}