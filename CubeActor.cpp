#include "CubeActor.h"
#include "MeshComponent.h"
#include "WallComponent.h"
#include "Assets.h"
#include "Game.h"

CubeActor::CubeActor()
{

	Mesh* mesh = &Assets::getMesh("Mesh_Cube");
	MeshComponent* mc = new MeshComponent(this);
	mc->setMesh(*mesh);
	box = new BoxComponent(this);
	box->setObjectBox(mesh->getBox()); 
	new WallComponent(this);
	getGame().addCube(this);
}

CubeActor::~CubeActor()
{
	getGame().removeCube(this);
}
