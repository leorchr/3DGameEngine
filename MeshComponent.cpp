#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Mesh.h"

MeshComponent::MeshComponent(Actor* owner) : Component(owner), mesh(nullptr), textureIndex(0), isVisible(true)
{
	owner->getGame().getRenderer().addMesh(this);
}

MeshComponent::~MeshComponent()
{
	owner.getGame().getRenderer().removeMesh(this);
}

void MeshComponent::setVisible(bool isVisibleP)
{
	isVisible = isVisibleP;
}

void MeshComponent::draw(Shader& shader)
{
}

void MeshComponent::setMesh(BasicMesh& meshP)
{
	mesh = &meshP;
}

void MeshComponent::setTextureIndex(size_t textureIndexP)
{
	textureIndex = textureIndexP;
}
