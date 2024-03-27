#include "ArrowActor.h"

ArrowActor::ArrowActor()
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));
	setScale(Vector3(50.0f, 5.0f, 5.0f));
	setRotation(Quaternion::identity);
	setPosition(Vector3(100.0f, 0.0f, -60.0f));
}

void ArrowActor::updateActor(float dt)
{
	Quaternion newRotation = getRotation();
	float angle = 5 * dt;
	Quaternion increment(Vector3::unitZ, angle);
	newRotation = Quaternion::concatenate(newRotation, increment);
	setRotation(newRotation);
}
