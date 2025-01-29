#include "imgui.h"

struct ImGUISettings
{
	static ImVec2 viewportPosition;
	static ImVec2 viewportSize;
	static ImVec2 outlinerPos;
	static ImVec2 outlinerSize;
	static ImVec2 outlinerChildSize;
	static int outlinerListSize;
	static ImVec2 playModePos;
	static ImVec2 playModeSize;
	static ImVec2 playModeButtonSize;
	
	static void updateForResolution(int windowWidth, int windowHeight)
	{
		if (windowWidth == 1920 && windowHeight == 1080) {
			viewportPosition = ImVec2(windowWidth - 550, 50);
			viewportSize = ImVec2(500, 300);
			outlinerPos = ImVec2(50, 50);
			outlinerSize = ImVec2(250, 280);
			outlinerChildSize = ImVec2(350, 800);
			outlinerListSize = 10;
			playModePos = ImVec2(windowWidth / 2 - 100, 50);
			playModeSize = ImVec2(125, 0);
			playModeButtonSize = ImVec2(100, 0);
		}
		
		else if (windowWidth == 1280 && windowHeight == 720) {
			viewportPosition = ImVec2(windowWidth - 375, 50);
			viewportSize = ImVec2(350, 250);
			outlinerPos = ImVec2(25, 50);
			outlinerSize = ImVec2(175, 250);
			outlinerChildSize = ImVec2(235, 800);
			outlinerListSize = 9;
			playModePos = ImVec2(windowWidth / 2 - 100, 50);
			playModeSize = ImVec2(125, 0);
			playModeButtonSize = ImVec2(100, 0);
		}
	}
};

ImVec2 ImGUISettings::viewportPosition = ImVec2(0, 0);
ImVec2 ImGUISettings::viewportSize = ImVec2(0, 0);
ImVec2 ImGUISettings::outlinerPos = ImVec2(0, 0);
ImVec2 ImGUISettings::outlinerSize = ImVec2(0, 0);
int ImGUISettings::outlinerListSize = 0;
ImVec2 ImGUISettings::outlinerChildSize = ImVec2(0, 0);
ImVec2 ImGUISettings::playModePos = ImVec2(0, 0);
ImVec2 ImGUISettings::playModeSize = ImVec2(0, 0);
ImVec2 ImGUISettings::playModeButtonSize = ImVec2(0, 0);
