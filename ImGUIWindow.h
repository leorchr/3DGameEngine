﻿#pragma once

class ImGUIWindow
{
public:
	ImGUIWindow();

	void update();
	void setActor(class Actor* actor);
	void setViewportActor(class ViewportActor* actor);
	void setShowImGUI(bool showImGUI);

protected:
	void viewport();
	void playmode();
	
private:
	class Actor* sphere;
	class ViewportActor* viewportActor;
	float position;

	// Viewport Actor
	float speed;

	bool showImGUI;
};