#include "MeshComponent.h"
#include "Mesh.h"
#include "Actor.h"
#include "Assets.h"
#include "Game.h"
#include "imgui.h"
#include "Log.h"
#include "Texture.h"
#include <assert.h>
#include <GL/glew.h>

MeshComponent::MeshComponent(Actor* owner) : Component(owner), mesh(nullptr), isVisible(true)
{
	owner->getGame().getRenderer().addMesh(this);
	setType(ComponentType::Mesh);
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
	if(mesh == nullptr) return;
	std::vector<MeshEntry> meshes = *mesh->getMeshes();
	if(meshes.size() == 0) return;
	
	Matrix4 worldTransform = owner.getWorldTransform();
	shader.setFloat("uSpecPower", 32.0f);
	shader.setMatrix4("uWorldTransform", worldTransform, false);

	glBindVertexArray(mesh->getVAO());

	for (unsigned int i = 0 ; i < meshes.size() ; i++) {
		unsigned int MaterialIndex = meshes[i].MaterialIndex;
		if(textures[MaterialIndex] == nullptr) continue;
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

void MeshComponent::setMesh(Mesh& meshP)
{
	mesh = &meshP;
	textures = *mesh->getTextures();
}

void MeshComponent::setTexture(int index, Texture* newTexture)
{
	if (index >= 0 && index < static_cast<int>(textures.size()))
	{
		textures[index] = newTexture;
	}
	else
	{
		Log::error(LogCategory::Application, "No Object Link To This Index");
	}
}

void MeshComponent::load(const rapidjson::Value& data)
{
	if (data.HasMember("Mesh"))
	{
		if(data["Mesh"].IsString())
		{
			if(mesh != nullptr) setMesh(Assets::getMesh(data["Name"].GetString()));
		}
	}
}

void MeshComponent::save(rapidjson::Value& actorAttributes, rapidjson::MemoryPoolAllocator<>& allocator)
{
	Component::save(actorAttributes, allocator);
	if(mesh != nullptr)
	{
		for(auto pair : Assets::meshes)
		{
			if(pair.second.getId() == mesh->getId())
			{
				rapidjson::Value meshValue;
				meshValue.SetString(pair.first.c_str(), allocator);
				actorAttributes.AddMember("Mesh", meshValue, allocator);
			}
		}
	}
}

void MeshComponent::updateImGUIOutliner()
{
	Component::updateImGUIOutliner();
	
	static int currentMeshSelected = 0;
						
	vector<const char*> meshesNames;
	meshesNames.reserve(Assets::meshes.size());
	for(const auto& pair : Assets::meshes)
	{
		meshesNames.emplace_back(pair.first.c_str());
	}
						
	if(ImGui::Combo("Meshes", &currentMeshSelected, meshesNames.data(), meshesNames.size(), 9))
	{
		setMesh(Assets::getMesh(meshesNames[currentMeshSelected]));
	}					
	static std::vector<int> currentTextureSelections;
	if (currentTextureSelections.size() != getTextures()->size())
	{
		currentTextureSelections.resize(getTextures()->size(), 0);
	}
					
	vector<const char*> textureNames;
	textureNames.reserve(Assets::textures.size());
	for(const auto& pair : Assets::textures)
	{
		textureNames.emplace_back(pair.first.c_str());
	}
	for(size_t i = 0; i < getTextures()->size(); i++)
	{
		ImGui::PushID(static_cast<int>(i));
		std::string label = "Texture " + std::to_string(i+1);
		if(ImGui::Combo(label.c_str(), &currentTextureSelections[i], textureNames.data(), static_cast<int>(textureNames.size()), 9))
		{
			setTexture(static_cast<int>(i), &Assets::getTexture(textureNames[currentTextureSelections[i]]));
		}
		ImGui::PopID();
	}
}