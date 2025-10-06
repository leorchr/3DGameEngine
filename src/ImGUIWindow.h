#ifdef _DEBUG

#pragma once
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
	void reset();
	void deleteSelectedActor();
	static bool showDemoWindow;
	static bool showStyleEditor;
	static int selectedActorIndex;
	static bool showRendererPostProcessWindow;
	static bool useSnap;
protected:
	void viewport();
	void outliner();
	void playmode();
	void imguizmo();
	static void postProcessing();
	
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

	// Im GUI
};

#endif