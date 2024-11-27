#include "ImGUIManager.h"
#include <imgui.h>

bool ImGUIManager::initialize()
{
	IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//ImGui_ImplSDL2_InitForOpenGL(Game::instance().getWindow().getSDLWindow(), Game::instance().getRenderer().getSdlGlContext());
	///ImGui_ImplOpenGL3_Init();
	
	return true;
}