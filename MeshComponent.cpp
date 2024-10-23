#include "MeshComponent.h"
#include "BasicMesh.h"
#include "Actor.h"
#include "Game.h"
#include "Texture.h"
#include <assert.h>
#include <GL/glew.h>

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

	std::vector<BasicMeshEntry> meshes = *mesh->getMeshes();
	std::vector<Texture*> textures = *mesh->getTextures();
	
	glBindVertexArray(mesh->getVAO());

	for (unsigned int i = 0 ; i < meshes.size() ; i++) {
		unsigned int MaterialIndex = meshes[i].MaterialIndex;

		assert(MaterialIndex < textures.size());

		if (textures[MaterialIndex]) {
			textures[MaterialIndex]->bind(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
								 meshes[i].NumIndices,
								 GL_UNSIGNED_INT,
								 (void*)(sizeof(unsigned int) * meshes[i].BaseIndex),
								 meshes[i].BaseVertex);
	}
	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}

void MeshComponent::setMesh(BasicMesh& meshP)
{
	mesh = &meshP;
}