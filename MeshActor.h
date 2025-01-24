#pragma once
#include "Actor.h"
#include <string>
#include <memory>

class MeshActor : public Actor
{
public:
	MeshActor(const std::string meshName);
	MeshActor();
	virtual ~MeshActor() override;
	MeshActor(const Actor&) = delete;
	MeshActor& operator=(const MeshActor&) = delete;

	std::shared_ptr<class MeshComponent> getMeshComponent();

	void load(const rapidjson::Value& data) override;

protected:
	std::shared_ptr<class MeshComponent> meshComponent;
};
