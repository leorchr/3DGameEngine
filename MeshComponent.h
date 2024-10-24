#pragma once
#include "Component.h"

class Mesh;

class MeshComponent : public Component
{
public:
	MeshComponent(Actor* owner);
	virtual ~MeshComponent();

	bool getVisible() const { return isVisible; }
	void setVisible(bool isVisibleP);

	virtual void draw(class Shader& shader);
	virtual void setMesh(class Mesh& meshP);


protected:
	Mesh* mesh;
	bool isVisible;
};

