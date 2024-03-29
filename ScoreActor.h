#pragma once
#include "Actor.h"
class ScoreActor : public Actor
{
public:
	ScoreActor();
	void setScore(int score);

private:
	class MeshComponent* mc;
};