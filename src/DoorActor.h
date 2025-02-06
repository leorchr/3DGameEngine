#pragma once
#include "Actor.h"

class DoorActor : public Actor
{
public:
	DoorActor();

	class BoxComponent* getBox() { return box; }
	void updateActor(float dt) override;

	void open();
	void close();
private:
	class MeshComponent* mc;
	class BoxComponent* box;
};
