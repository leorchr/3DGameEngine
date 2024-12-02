#ifdef _DEBUG

#include "ImGUIWindow.h"
#include "Actor.h"
#include "Game.h"
#include "ViewportActor.h"
#include "imgui.h"
#include "MoveComponent.h"

ImGUIWindow::ImGUIWindow(std::vector<class Actor*>& actors, std::vector<std::string>& actorNames) : sphere(nullptr), viewportActor(nullptr), position(0.0f), speed(0.0f), showImGUI(true), actors(actors), actorNames(actorNames) {}

void ImGUIWindow::update()
{
	if(showImGUI)
	{
		viewport();
		outliner();
		playmode();
		//ImGui::ShowStyleEditor();
		ImGui::ShowDemoWindow();
	}
}

void ImGUIWindow::setActor(Actor* actor)
{
	sphere = actor;
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
	ImGui::SetNextWindowPos(ImVec2(50.0f, WINDOW_HEIGHT - 250.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.0f, 200.0f), ImGuiCond_Always);
	
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::BeginTabBar("Panel"))
	{
		
		if (ImGui::BeginTabItem("Viewport"))
		{
			if(sphere)
			{
				ImGui::Text("Sphere : ");
				
				Vector3 currentPosition = sphere->getPosition();
				Vector3 uiPosition = currentPosition;
					
				if (ImGui::DragFloat3("Position", &uiPosition.x, 1.0f)) {
					// Vérifier si la position a changé
					if (uiPosition != currentPosition) {
						sphere->setPosition(uiPosition);
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
						
						sphere->setRotation(rot);
					}
				}

				// Scale
				
				Vector3 currentScale = sphere->getScale();
				Vector3 uiScale = currentScale;
				
				if (ImGui::DragFloat3("Scale", &uiScale.x, 0.1f)) {
					if (uiScale != currentScale) {
						if (lockScale)
						{
							if (uiScale.x != currentScale.x)
							{
								uiScale.y = uiScale.x;
								uiScale.z = uiScale.x;
							}
							if (uiScale.y != currentScale.y)
							{
								uiScale.x = uiScale.y;
								uiScale.z = uiScale.y;
							}
							if (uiScale.z != currentScale.z)
							{
								uiScale.x = uiScale.z;
								uiScale.y = uiScale.z;
							}
						}
						sphere->setScale(uiScale);
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Lock", &lockScale);
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
	ImGui::SetNextWindowPos(ImVec2(50.0f, WINDOW_HEIGHT - 750.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.0f, 200.0f), ImGuiCond_Always);
	
	ImGui::Begin("Outliner", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	// Construction de la const char* pour ImGUI
	static int selectedActorIndex = -1;
	
	std::vector<const char*> itemNames;
	itemNames.reserve(actorNames.size());
	for (const auto& name : actorNames) {
	 	itemNames.push_back(name.c_str());
	}

	// Fin de la construction
	
	ImGui::ListBox("Actors", &selectedActorIndex, itemNames.data(), itemNames.size());
	ImGui::End();
}

void ImGUIWindow::playmode()
{
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 190.0f , 25.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(175.0f, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Playmode", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::Button("Play", ImVec2(150.0f, 0.0f)))
	{
		Game::instance().setMode(EngineMode::Game);
	}
	ImGui::End();
}

#endif