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
		ImGui::Begin("Viewport", &showImGUI);
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