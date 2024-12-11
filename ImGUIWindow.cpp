#include "Assets.h"
#include "MeshComponent.h"
#include <iostream>
#ifdef _DEBUG

#include "ImGUIWindow.h"
#include "Actor.h"
#include "Game.h"
#include "ViewportActor.h"
#include "imgui.h"
#include "MoveComponent.h"

ImGUIWindow::ImGUIWindow(std::vector<class Actor*>& actors, std::vector<std::string>& actorNames) : currentActor(nullptr), viewportActor(nullptr), position(0.0f), speed(0.0f), showImGUI(true), actors(actors), actorNames(actorNames) {}

void ImGUIWindow::update()
{
	if(showImGUI)
	{
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

void ImGUIWindow::viewport()
{
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 550.0f, 50.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.0f, 220.0f), ImGuiCond_Always);
	
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::BeginTabBar("Panel"))
	{
		
		if (ImGui::BeginTabItem("Viewport"))
		{
			if(currentActor)
			{
				ImGui::Text(currentActor->getName().c_str());
				
				Vector3 currentPosition = currentActor->getPosition();
				Vector3 uiPosition = currentPosition;
					
				if (ImGui::DragFloat3("Position", &uiPosition.x, 1.0f)) {
					// Vérifier si la position a changé
					if (uiPosition != currentPosition) {
						currentActor->setPosition(uiPosition);
					}
				}


				//Rotation
				Vector3 currentRotation = uiRotation;

				if (ImGui::DragFloat3("Rotation", &uiRotation.x, 1.0f)) {
					if (uiRotation != currentRotation) {

						// ZYX order for rotations
						Quaternion rot = Quaternion::identity;
						
						Quaternion yaw = Quaternion(Vector3::unitZ, uiRotation.z*(Maths::pi/180));
						rot = Quaternion::concatenate(yaw, rot);
						
						Quaternion pitch = Quaternion(Vector3::unitY, uiRotation.y*(Maths::pi/180));
						rot = Quaternion::concatenate(pitch, rot);
						
						Quaternion roll = Quaternion(Vector3::unitX, uiRotation.x*(Maths::pi/180));
						rot = Quaternion::concatenate(roll, rot);
						
						currentActor->setRotation(rot);
					}
				}

				// Scale
				
				Vector3 currentScale = currentActor->getScale();
				Vector3 uiScale = currentScale;
				
				if (ImGui::DragFloat3("Scale", &uiScale.x, 0.1f)) {
					if (uiScale != currentScale) {
						if (lockScale)
						{
							if (uiScale.x != currentScale.x)
							{
								float difference = uiScale.x - currentScale.x;
								uiScale.y += difference;
								uiScale.z += difference;
							}
							if (uiScale.y != currentScale.y)
							{
								float difference = uiScale.y- currentScale.y;
								uiScale.x += difference;
								uiScale.z += difference;
							}
							if (uiScale.z != currentScale.z)
							{
								float difference = uiScale.z - currentScale.z;
								uiScale.x += difference;
								uiScale.y += difference;
							}
						}
						currentActor->setScale(uiScale);
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Lock", &lockScale);

				
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
			
			std::vector<const char*> itemNames;
			itemNames.reserve(actorNames.size());
			for (const auto& name : actorNames) {
	 			itemNames.push_back(name.c_str());
			}

			// Fin de la construction
			ImGui::BeginChild("NoScrollChild", ImVec2(345, 900), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::ListBox("##Actors", &selectedActorIndex, itemNames.data(), itemNames.size(), 9);
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

#endif