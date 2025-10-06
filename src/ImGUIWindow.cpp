#include "imgui_internal.h"
#ifdef _DEBUG

#include "ActorFactory.h"
#include "ImGUIWindow.h"
#include "Component.h"
#include "Actor.h"
#include "Game.h"
#include "SaveSystem.h"
#include "ViewportActor.h"
#include "ImGUISettings.h"
#include "RendererOGL.h"

#include <imgui.h>

#include "ImGuizmo.h"

bool ImGUIWindow::showDemoWindow = false;
bool ImGUIWindow::showStyleEditor = false;
int ImGUIWindow::selectedActorIndex = -1;
bool ImGUIWindow::showRendererPostProcessWindow = false;

ImGUIWindow::ImGUIWindow(std::vector<class Actor*>& actors) : currentActor(nullptr), viewportActor(nullptr), position(0.0f), speed(0.0f), showImGUI(true), actors(actors)
{
	updateItems();
	ImGUISettings::updateForResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
	
}

void ImGUIWindow::update()
{
	if(showImGUI)
	{
		menu();
		viewport();
		outliner();
		playmode();
		imguizmo();
		if(showDemoWindow) ImGui::ShowDemoWindow();
		if(showStyleEditor) ImGui::ShowStyleEditor();
		postProcessing();
	}
}

void ImGUIWindow::setActor(Actor* actor)
{
	currentActor = actor;
}

void ImGUIWindow::setViewportActor(ViewportActor* actor)
{
	viewportActor = actor;
	if(viewportActor) speed = viewportActor->getBaseSpeed();
}

void ImGUIWindow::setShowImGUI(bool showImGUI)
{
	this->showImGUI = showImGUI;
}

void ImGUIWindow::updateItems()
{
	imGuiActors.clear();
	itemNames.clear();
	itemNamePtrs.clear();
	
	imGuiActors.reserve(actors.size());
	for (const auto& actor : actors) {
		if(actor->getTypeName() != "ViewportActor" && actor->getState() == Actor::ActorState::Active) imGuiActors.push_back(actor);
	}
	
	itemNames.reserve(actors.size());
	itemNamePtrs.reserve(actors.size());
	for (const auto& actor : imGuiActors) {
		itemNames.push_back(actor->getName());
	}
	for (const auto& name : itemNames) {
		itemNamePtrs.push_back(name.c_str());
	}
}

void ImGUIWindow::reset()
{
	selectedActorIndex = -1;
}

void ImGUIWindow::deleteSelectedActor()
{
	if(currentActor != nullptr) currentActor->setState(Actor::ActorState::Dead);
	reset();
}

void ImGUIWindow::viewport()
{
	ImGui::SetNextWindowPos(ImGUISettings::viewportPosition, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImGUISettings::viewportSize, ImGuiCond_Always);
	
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::BeginTabBar("Panel"))
	{
		
		if (ImGui::BeginTabItem("Viewport"))
		{
			if(currentActor != nullptr)
			{
				if(currentActor->getState() == Actor::ActorState::Active)
				{
					currentActor->updateImGUIOutliner();
					if(!currentActor->getComponents().empty())ImGui::Text("Components");
					for(auto component : currentActor->getComponents())
					{
						component->updateImGUIOutliner();
					}
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("World Settings"))
		{
			if(viewportActor)
			{
				viewportActor->updateImGUIOutliner();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void ImGUIWindow::outliner()
{
	ImGui::SetNextWindowPos(ImGUISettings::outlinerPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImGUISettings::outlinerSize, ImGuiCond_Always);

	ImGui::Begin("Outliner", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	if(ImGui::BeginTabBar("Outliner"))
	{
		if (ImGui::BeginTabItem("Outliner"))
		{
			ImGui::BeginChild("NoScrollChild", ImGUISettings::outlinerChildSize, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::ListBox("##Actors", &selectedActorIndex, itemNamePtrs.data(), itemNamePtrs.size(), ImGUISettings::outlinerListSize);
			if (selectedActorIndex != -1) {
				currentActor = imGuiActors[selectedActorIndex];
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

}

void ImGUIWindow::playmode()
{
	ImGui::SetNextWindowPos(ImGUISettings::playModePos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGUISettings::playModeSize, ImGuiCond_Always);
	ImGui::Begin("Playmode", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::Button("Play", ImGUISettings::playModeButtonSize))
	{
		Game::instance().setMode(EngineMode::Game);
	}
	ImGui::End();
}

void ImGUIWindow::imguizmo()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar;
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH,WINDOW_HEIGHT));
	
	ImGui::Begin("Scene", nullptr, flags);

	ImVec2 winPos   = ImGui::GetWindowPos();
	ImVec2 rMin     = ImGui::GetWindowContentRegionMin();
	ImVec2 rMax     = ImGui::GetWindowContentRegionMax();
	ImVec2 p0       = ImVec2(winPos.x + rMin.x, winPos.y + rMin.y);
	ImVec2 p1       = ImVec2(winPos.x + rMax.x, winPos.y + rMax.y);
	ImVec2 size     = ImVec2(p1.x - p0.x, p1.y - p0.y);

	ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
	ImGuizmo::SetRect(p0.x, p0.y, size.x, size.y);

	if(currentActor != nullptr)
	{
		if(currentActor->getState() == Actor::ActorState::Active)
		{
			currentActor->updateImGuizmo();
		}
	}

	ImGui::End();
}

void ImGUIWindow::postProcessing()
{
	Game::instance().getRenderer().getPostProcess()->updateImGui();
}

void ImGUIWindow::menu()
{
	static int activeTab = 0;
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			SaveSystem::save();
		}
		if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
		{
			SaveSystem::saveAs();
		}
		if (ImGui::MenuItem("Load", "Ctrl+L"))
		{
			SaveSystem::loadFile();
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Show Demo Window", nullptr, showDemoWindow))
		{
			showDemoWindow = !showDemoWindow; // Toggle the value
		}

		if (ImGui::MenuItem("Show Style Editor", nullptr, showStyleEditor))
		{
			showStyleEditor = !showStyleEditor; // Toggle the value
		}

		if (ImGui::MenuItem("Show Compute Shader Editor", nullptr, showRendererPostProcessWindow))
		{
			showRendererPostProcessWindow = !showRendererPostProcessWindow; // Toggle the value
			PostProcessing::setPostProcessWindowActive(showRendererPostProcessWindow);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Add"))
	{
		for(auto& pair : ActorFactory::getInstance().getCreators())
		{
			if (ImGui::MenuItem(pair.first.c_str(), nullptr))
			{
				ActorFactory::getInstance().create(pair.first);
			}
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

#endif
