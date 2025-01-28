#include "imgui.h"

struct ImguiSettings
{
	static ImVec2 viewportPosition;
	static ImVec2 viewportSize;
	static ImVec2 outlinerPos;
	static ImVec2 outlinerSize;
	static ImVec2 outlinerChildSize;
	static ImVec2 playModePos;
	static ImVec2 playModeSize;
	static ImVec2 playModeButtonSize;
	
	static void updateForResolution(int windowWidth, int windowHeight)
	{
		if (windowWidth == 1920 && windowHeight == 1080) {
			viewportPosition = ImVec2(windowWidth - 550.0f,50.0f);
			viewportSize = ImVec2(500, 300);
			outlinerPos = ImVec2(50.0f, 50.0f);
			outlinerSize = ImVec2(250.0f, 300.0f);
			outlinerChildSize = ImVec2(300.0f, 170.0f);
			playModePos = ImVec2(windowWidth/2-100, 50.0f);
			playModeSize = ImVec2(125	.0f, 0.0f);
			playModeButtonSize = ImVec2(100.0f, 0.0f);
		}
		else if (windowWidth == 1280 && windowHeight == 720) {
			viewportPosition = ImVec2(50, 50);
			viewportSize = ImVec2(250, 120);
			outlinerPos = ImVec2(50.0f, 50.0f);
			outlinerSize = ImVec2(50.0f, 50.0f);
			outlinerChildSize = ImVec2(300.0f, 170.0f);
			playModePos = ImVec2(windowWidth/2-100, 50.0f);
			playModeSize = ImVec2(100.0f, 0.0f);
			playModeButtonSize = ImVec2(100.0f, 0.0f);
		}
	}
};

ImVec2 ImguiSettings::viewportPosition = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::viewportSize = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::outlinerPos = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::outlinerSize = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::outlinerChildSize = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::playModePos = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::playModeSize = ImVec2(0.0f, 0.0f);
ImVec2 ImguiSettings::playModeButtonSize = ImVec2(0.0f, 0.0f);