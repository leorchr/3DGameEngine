#include "CubeActor.h"
#include "Assets.h"
#include "Game.h"

CubeActor::CubeActor()
{
	Mesh* mesh = &Assets::getMesh("Monkey");
	mc = new MeshComponent(this);
	mc->setMesh(*mesh);
	box = new BoxComponent(this); 
	getGame().addCube(this);
}

CubeActor::~CubeActor()
{
	getGame().removeCube(this);
}