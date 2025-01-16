#ifdef _DEBUG

#include "ImGUIWindow.h"
#include "Actor.h"
#include "Game.h"
#include "ViewportActor.h"
#include "imgui.h"
#include "MoveComponent.h"
#include "Assets.h"
#include "MeshComponent.h"
#include "Texture.h"
#include "SaveSystem.h"
#include <iostream>

ImGUIWindow::ImGUIWindow(std::vector<class Actor*>& actors) : currentActor(nullptr), viewportActor(nullptr), position(0.0f), speed(0.0f), showImGUI(true), actors(actors)
{
	itemNames.reserve(actors.size());
	itemNamePtrs.reserve(actors.size());
	for (const auto& actor : actors) {
		itemNames.push_back(actor->getName());
	}
	for (const auto& name : itemNames) {
		itemNamePtrs.push_back(name.c_str());
	}
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
		//ImGui::ShowStyleEditor();
		//ImGui::ShowDemoWindow();
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
	itemNames.clear();
	itemNamePtrs.clear();

	for (const auto& actor : actors) {
		itemNames.push_back(actor->getName());
	}
	for (const auto& name : itemNames) {
		itemNamePtrs.push_back(name.c_str());
	}
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
			if(currentActor)
			{
				currentActor->updateImGUIOutliner();
				if(!currentActor->getComponents().empty())ImGui::Text("Components");
				for(auto component : currentActor->getComponents())
				{
					if(component->getType() == ComponentType::Mesh)
					{
						static int currentMeshSelected = 0;
						
						vector<const char*> meshesNames;
						meshesNames.reserve(Assets::meshes.size());
						for(const auto& pair : Assets::meshes)
						{
							meshesNames.emplace_back(pair.first.c_str());
						}
						
						if(ImGui::Combo("Meshes", &currentMeshSelected, meshesNames.data(), meshesNames.size(), 9))
						{
							if(auto mc = dynamic_cast<MeshComponent*>(component))
							{
								mc->setMesh(Assets::getMesh(meshesNames[currentMeshSelected]));
							}
						}

						if(auto mc = dynamic_cast<MeshComponent*>(component))
						{
							
						static std::vector<int> currentTextureSelections;
						if (currentTextureSelections.size() != mc->getTextures()->size())
						{
							currentTextureSelections.resize(mc->getTextures()->size(), 0);
						}
						
						vector<const char*> textureNames;
						textureNames.reserve(Assets::textures.size());
						for(const auto& pair : Assets::textures)
						{
							textureNames.emplace_back(pair.first.c_str());
						}
							for(size_t i = 0; i < mc->getTextures()->size(); i++)
							{
								ImGui::PushID(static_cast<int>(i));
								std::string label = "Texture " + std::to_string(i+1);
								if(ImGui::Combo(label.c_str(), &currentTextureSelections[i], textureNames.data(), static_cast<int>(textureNames.size()), 9))
								{
									mc->setTexture(static_cast<int>(i), &Assets::getTexture(textureNames[currentTextureSelections[i]]));
								}
								ImGui::PopID();
							}
						}
					}
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("World Settings"))
		{
			if(viewportActor)
			{
				ImGui::Text("Camera Settings : ");
				Vector3 currentPosition = viewportActor->getPosition();
				Vector3 uiPosition = currentPosition;
				if(ImGui::DragFloat3("Camera Position", &uiPosition.x, 1.0f))
				{
					if (uiPosition != currentPosition) {
						viewportActor->setPosition(uiPosition);
					}
				}
				if(ImGui::DragFloat("Camera Speed", &speed, 1.0f, 0.0f, FLT_MAX))
				{
					viewportActor->setBaseSpeed(speed);
				}
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
			static int selectedActorIndex = -1;
			
			// Fin de la construction
			ImGui::BeginChild("NoScrollChild", ImVec2(345, 900), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::ListBox("##Actors", &selectedActorIndex, itemNamePtrs.data(), itemNamePtrs.size(), 9);
			if (selectedActorIndex != -1) {
				currentActor = actors[selectedActorIndex];
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
	ImGui::EndMainMenuBar();
}

#endif
