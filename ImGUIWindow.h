#ifdef _DEBUG

#pragma once
#include "Vector3.h"
#include <string>
#include <vector>

class ImGUIWindow
{
public:
	ImGUIWindow(std::vector<class Actor*>& actors);

	void menu();
	void update();
	void setActor(class Actor* actor);
	void setViewportActor(class ViewportActor* actor);
	void setShowImGUI(bool showImGUI);
	void updateItems();

protected:
	void viewport();
	void outliner();
	void playmode();
	void addActor();
	
private:
	class Actor* currentActor;
	class ViewportActor* viewportActor;
	float position;

	// Viewport Actor
	float speed;
	bool showImGUI;

	// Refers to game lists
	std::vector<class Actor*>& actors;
	
	std::vector<class Actor*> imGuiActors;
	std::vector<std::string> itemNames;
	std::vector<const char*> itemNamePtrs;
};

#endif