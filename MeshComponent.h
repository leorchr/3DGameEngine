#pragma once
#include "Component.h"
#include <vector>

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

	Mesh* getMesh() { return mesh; }
	std::vector<class Texture*>* getTextures() {return &textures; }
	void setTexture(int index, class Texture* newTexture);

#ifdef  _DEBUG
	virtual void updateImGUIOutliner() override;
#endif
	
protected:
	Mesh* mesh;
	std::vector<class Texture*> textures;

	bool isVisible;
};

