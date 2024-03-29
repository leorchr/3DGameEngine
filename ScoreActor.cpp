#include "ScoreActor.h"
#include "MeshComponent.h"
#include "Assets.h"

ScoreActor::ScoreActor() :
	Actor()
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Score"));
}

void ScoreActor::setScore(int score)
{
	mc->setTextureIndex(score);
}
