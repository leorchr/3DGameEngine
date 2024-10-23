#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Mesh.h"

MeshComponent::MeshComponent(Actor* owner) : Component(owner), mesh(nullptr), isVisible(true)
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
	Matrix4 worldTransform = owner.getWorldTransform();
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "uWorldTransform"), 1, GL_FALSE, worldTransform.getAsFloatPtr());
	mesh->Render();
}

void MeshComponent::setMesh(BasicMesh& meshP)
{
	mesh = &meshP;
}