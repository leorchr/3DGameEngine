#include "ArrowActor.h"

ArrowActor::ArrowActor(float rotationSpeedP, float scaleSpeedP) :
	rotationSpeed(rotationSpeedP),
	scaleSpeed(scaleSpeedP)
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));

	currentState = 1;
	baseScale = Vector3(50.0f, 5.0f, 5.0f);
	setScale(baseScale);
	setRotation(Quaternion::identity);
	setPosition(Vector3(80.0f, 0.0f, -60.0f));

	//Variable init
	newRotation = Quaternion::identity;
	increment = Quaternion::identity;
	angle = 0;
	yaw = 0;
	scaleY = 0;
	maxScaleY = 15;
	minScaleY = 4;
}

void ArrowActor::updateActor(float dt)
{
	switch (currentState)
	{
	case 1:
	default:
		newRotation = getRotation();
		angle = rotationSpeed * dt;
		increment = Quaternion(Vector3::unitZ, angle);
		newRotation = Quaternion::concatenate(newRotation, increment);
		yaw = Maths::asin(2 * newRotation.x * newRotation.y + 2 * newRotation.z * newRotation.w);

		if (Maths::toDegrees(yaw) > 45 && rotationSpeed > 0) rotationSpeed = -Maths::abs(rotationSpeed);
		if (Maths::toDegrees(yaw) < -45 && rotationSpeed < 0) rotationSpeed = Maths::abs(rotationSpeed);

		setRotation(newRotation);
		break;

	case 2:

		if (baseScale.y + scaleY > maxScaleY && scaleSpeed > 0) scaleSpeed = -Maths::abs(scaleSpeed);
		if (baseScale.y + scaleY < minScaleY && scaleSpeed < 0) scaleSpeed = Maths::abs(scaleSpeed);

		scaleY += dt * scaleSpeed;
		setScale(Vector3(baseScale.x, baseScale.y + scaleY, baseScale.z));
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		scaleY = 0;
		setScale(baseScale);
		currentState = 1;
		break;
	}
}

void ArrowActor::setVisible(bool isVisible) {
	mc->setVisible(isVisible);
}

void ArrowActor::setCurrentState(int currentStateP) {
	currentState = currentStateP;
}