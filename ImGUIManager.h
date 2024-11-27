#pragma once

class ImGUIManager
{
public:
	ImGUIManager() = delete;
	ImGUIManager(const ImGUIManager&) = delete;
	ImGUIManager& operator=(const ImGUIManager&) = delete;

	static bool initialize();
	static void beginDraw();
	static void render();
};