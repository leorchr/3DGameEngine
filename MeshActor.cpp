#include "MeshActor.h"
#include "MeshComponent.h"
#include "Assets.h"

MeshActor::MeshActor(const std::string meshName) : Actor(), meshComponent(nullptr)
{
	meshComponent = std::make_shared<MeshComponent>(this);
	meshComponent->setMesh(Assets::getMesh(meshName));
	setPosition(Vector3(0.0f,0.0f,15.0f));
}

MeshActor::MeshActor() : Actor(), meshComponent(nullptr)
{
	meshComponent = std::make_shared<MeshComponent>(this);
	setPosition(Vector3(0.0f,0.0f,15.0f));
}

MeshActor::~MeshActor() {}

std::shared_ptr<MeshComponent> MeshActor::getMeshComponent()
{
	return meshComponent;
}

void MeshActor::load(const rapidjson::Value& data)
{
	Actor::load(data);

	if (data.HasMember("Mesh") && data["Mesh"].IsString())
	{
		string meshName = data["Name"].GetString();
		meshComponent->setMesh(Assets::getMesh(meshName));
	}
}