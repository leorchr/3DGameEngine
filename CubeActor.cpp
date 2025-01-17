#include "CubeActor.h"
#include "Assets.h"
#include "BoxComponent.h"
#include "Game.h"
#include "MeshComponent.h"

CubeActor::CubeActor()
{
	mc = new MeshComponent(this);
	Mesh* mesh = &Assets::getMesh("Cube");
	mc->setMesh(*mesh);
	box = new BoxComponent(this);
	box->setObjectBox(mesh->getBox());
	setName("Cube");
	getGame().addCube(this);
}

CubeActor::~CubeActor()
{
	getGame().removeCube(this);
}