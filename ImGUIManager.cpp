#include "ImGUIManager.h"
#include "Game.h"
#include "ImGUIWindow.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

bool ImGUIManager::initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplSDL2_InitForOpenGL(Game::instance().getWindow().getSDLWindow(), Game::instance().getRenderer().getSdlGlContext());
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGui::StyleColorsDark();
	return true;
}

void ImGUIManager::beginDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	Game::instance().getImGuiWindow()->update();
}

void ImGUIManager::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGUIManager::close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}