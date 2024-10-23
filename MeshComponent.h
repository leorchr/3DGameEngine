#pragma once
#include "Component.h"

class BasicMesh;

class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner);
	virtual ~MeshComponent();

	bool getVisible() const { return isVisible; }
	void setVisible(bool isVisibleP);

	virtual void draw(class Shader& shader);
	virtual void setMesh(class BasicMesh& meshP);
	void setTextureIndex(size_t textureIndexP);


protected:
	BasicMesh* mesh;
	size_t textureIndex;
	bool isVisible;
};

