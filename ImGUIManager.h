#ifdef _DEBUG

#pragma once

class ImGUIManager
{
public:
	ImGUIManager();
	ImGUIManager(const ImGUIManager&) = delete;
	ImGUIManager& operator=(const ImGUIManager&) = delete;
	
	static bool initialize();
	static void beginDraw();
	static void render();
	static void close();
};

#endif