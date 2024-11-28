#include "ImGUIWindow.h"
#include "Actor.h"
#include "Game.h"
#include "ViewportActor.h"
#include "imgui.h"
#include "MoveComponent.h"
#include <string>

ImGUIWindow::ImGUIWindow() : sphere(nullptr), viewportActor(nullptr), position(0.0f), speed(0.0f), showImGUI(true) {}

void ImGUIWindow::update()
{
	if(showImGUI)
	{
		viewport();
		playmode();
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(50.0f, 50.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.0f, 200.0f), ImGuiCond_Always);
	
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	if(sphere)
	{
		ImGui::Text("Sphere : ");
		Vector3 currentPosition = sphere->getPosition();
		Vector3 uiPosition = currentPosition;
			
		if (ImGui::SliderFloat3("Position Slider", &uiPosition.x, -500.0f, 500.0f)) {
			// Vérifier si la position a changé
			if (uiPosition != currentPosition) {
				sphere->setPosition(uiPosition);
			}
		}
			
	}
	if(viewportActor)
	{
		ImGui::Text("Camera Settings : ");
		Vector3 currentPosition = viewportActor->getPosition();
		Vector3 uiPosition = currentPosition;
		if(ImGui::SliderFloat3("Camera Position", &uiPosition.x, -500.0f, 500.0f))
		{
			if (uiPosition != currentPosition) {
				viewportActor->setPosition(uiPosition);
			}
		}
		if(ImGui::SliderFloat("Camera Speed", &speed, 0.0f, 500.0f))
		{
			viewportActor->setBaseSpeed(speed);
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);
}

void ImGUIWindow::playmode()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - 190.0f , 25.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(170.0f, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Playmode", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	if(ImGui::Button("Play", ImVec2(150.0f, 0.0f)))
	{
		Game::instance().setMode(EngineMode::Game);
	}
	ImGui::End();
	ImGui::PopStyleColor(7);
}