#ifdef _DEBUG

#include "ImGUIWindow.h"
#include "Component.h"
#include "Actor.h"
#include "Game.h"
#include "imgui.h"
#include "SaveSystem.h"
#include "ViewportActor.h"

bool ImGUIWindow::showDemoWindow = false;
bool ImGUIWindow::showStyleEditor = false;
int ImGUIWindow::selectedActorIndex = -1;


ImGUIWindow::ImGUIWindow(std::vector<class Actor*>& actors) : currentActor(nullptr), viewportActor(nullptr), position(0.0f), speed(0.0f), showImGUI(true), actors(actors)
{
	updateItems();
}

void ImGUIWindow::update()
{
	if(showImGUI)
	{
		menu();
		viewport();
		outliner();
		playmode();
		addActor();
		if(showDemoWindow) ImGui::ShowDemoWindow();
		if(showStyleEditor) ImGui::ShowStyleEditor();
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

void ImGUIWindow::viewport()
{
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 550.0f, 50.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.0f, 270.0f), ImGuiCond_Always);
	
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::BeginTabBar("Panel"))
	{
		
		if (ImGui::BeginTabItem("Viewport"))
		{
			if(currentActor && currentActor->getState() == Actor::ActorState::Active)
			{
				currentActor->updateImGUIOutliner();
				if(!currentActor->getComponents().empty())ImGui::Text("Components");
				for(auto component : currentActor->getComponents())
				{
					component->updateImGUIOutliner();
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
	ImGui::SetNextWindowPos(ImVec2(50.0f, 50.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250.0f, 250.0f), ImGuiCond_Always);

	ImGui::Begin("Outliner", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	if(ImGui::BeginTabBar("Outliner"))
	{
		if (ImGui::BeginTabItem("Outliner"))
		{
			// Construction de la const char* pour ImGUI
			
			// Fin de la construction
			ImGui::BeginChild("NoScrollChild", ImVec2(345, 900), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::ListBox("##Actors", &selectedActorIndex, itemNamePtrs.data(), itemNamePtrs.size(), 9);
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
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH/2 - 95.0f , 50.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(175.0f, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Playmode", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::Button("Play", ImVec2(150.0f, 0.0f)))
	{
		Game::instance().setMode(EngineMode::Game);
	}
	ImGui::End();
}

void ImGUIWindow::addActor()
{
	ImGui::SetNextWindowPos(ImVec2(50.0f , 350.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(175.0f, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Actors", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::Button("New Actor", ImVec2(150.0f, 0.0f)))
	{
		Game::instance().createActor();
	}
	ImGui::End();
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
		if (ImGui::MenuItem("Load"))
		{
			SaveSystem::load();
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
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

#endif
