#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "Component.h"
#include "imgui.h"
#include "ImGUIWindow.h"
#include "ImGuizmo.h"
#include "Maths.h"

Actor::Actor() :
	state(Actor::ActorState::Active),
	position(Vector3::zero),
	scale(Vector3(1.0f, 1.0f, 1.0f)),
	rotation(Quaternion::identity),
	mustRecomputeWorldTransform(true),
	game(Game::instance()),
	name("Unnamed")
{
	game.addActor(this);
}

Actor::~Actor()
{
	game.removeActor(this);
	while (!components.empty())
	{
		delete components.back();
	}
}

void Actor::setPosition(Vector3 positionP)
{
	position = positionP;
	mustRecomputeWorldTransform = true;
}

void Actor::setScale(Vector3 scaleP)
{
	scale = scaleP;
	mustRecomputeWorldTransform = true;
}

void Actor::setRotation(Quaternion rotationP)
{
	rotation = rotationP;
	mustRecomputeWorldTransform = true;
}

void Actor::rotate(const Vector3& axis, float angle)
{
	Quaternion newRotation = rotation;
	Quaternion increment(axis, angle);
	newRotation = Quaternion::concatenate(newRotation, increment);
	setRotation(newRotation);
}

void Actor::setAngle(const Vector3& axis, float angle)
{
	Quaternion newRotation(axis, angle);
	setRotation(newRotation);
}

void Actor::setState(ActorState stateP)
{
	state = stateP;
}

Vector3 Actor::getForward() const
{
	return Vector3::transform(Vector3::unitY, rotation);
}

Vector3 Actor::getRight() const
{
	return Vector3::transform(Vector3::unitX, rotation);
}

Vector3 Actor::getUp() const
{
	return Vector3::transform(Vector3::unitZ, rotation);
}

void Actor::computeWorldTransform()
{
	if (mustRecomputeWorldTransform)
	{
		mustRecomputeWorldTransform = false;
		worldTransform = Matrix4::createScale(scale);
		worldTransform *= Matrix4::createFromQuaternion(rotation);
		worldTransform *= Matrix4::createTranslation(position);

		for (auto component : components)
		{
			component->onUpdateWorldTransform();
		}
	}
}

void Actor::rotateToNewForward(const Vector3& newForward, const Quaternion& baseRotation, float lerpValue)
{
	// Figure out difference between original (unit x) and new
	float dot = Vector3::dot(Vector3::unitX, newForward);
	float angle = Maths::acos(dot);

	if (dot < 0.9999f && dot > -0.9999f)
	{
		// Rotate about axis from cross product
		Vector3 axis = Vector3::cross(Vector3::unitX, newForward);
		axis.normalize();
		Quaternion targetRotation = Quaternion(axis, angle);
		Quaternion step = Quaternion::slerp(baseRotation, targetRotation, lerpValue);
		setRotation(step);
	}

}

void Actor::rotateToNewForward(const Vector3& newForward)
{
	// Figure out difference between original (unit x) and new
	float dot = Vector3::dot(Vector3::unitY, newForward);
	float angle = Maths::acos(dot);
	// Facing down X
	if (dot > 0.9999f)
	{
		setRotation(Quaternion::identity);
	}
	// Facing down -X
	else if (dot < -0.9999f)
	{
		setRotation(Quaternion(Vector3::unitZ, Maths::pi));
	}
	else
	{
		// Rotate about axis from cross product
		Vector3 axis = Vector3::cross(Vector3::unitY, newForward);
		axis.normalize();
		setRotation(Quaternion(axis, angle));
	}
}

void Actor::processInput(const InputState& inputState)
{
	if (state == Actor::ActorState::Active)
	{
		for (auto component : components)
		{
			component->processInput(inputState);
		}
		actorInput(inputState);
	}
}

void Actor::actorInput(const InputState& inputState)
{
}

void Actor::updateInGame(float dt)
{
	if (state == Actor::ActorState::Active)
	{
		computeWorldTransform();
		updateComponents(dt);
		updateActor(dt);
		computeWorldTransform();
	}
}

void Actor::updateInEditor(float dt)
{
	if (state == Actor::ActorState::Active)
	{
		computeWorldTransform();
		computeWorldTransform();
	}
}

void Actor::updateComponents(float dt)
{
	for (auto component : components)
	{
		component->update(dt);
	}
}

void Actor::updateActor(float dt)
{
}

void Actor::addComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->getUpdateOrder();
	auto iter = begin(components);
	for (; iter != end(components); ++iter)
	{
		if (myOrder < (*iter)->getUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	components.insert(iter, component);
}

void Actor::removeComponent(Component* component)
{
	auto iter = std::find(begin(components), end(components), component);
	if (iter != end(components))
	{
		components.erase(iter);
	}
}

std::vector<Component*> Actor::getComponents()
{
	return components;
}

void Actor::setName(std::string name)
{
	this->name = name;
	game.updateImGUI();
}

std::string Actor::getTypeName() const
{
	const std::string prefix = "class ";
	std::string typeNameString = typeid(*this).name();
	if (typeNameString.find(prefix) == 0) {
		return typeNameString.substr(prefix.size());
	}
	return typeNameString;
}

#ifdef _DEBUG
static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::ROTATE);
static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
static bool useSnap(false);

void Actor::updateImGUIOutliner()
{
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        currentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_E))
        currentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        currentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
        currentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
        currentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
        currentGizmoOperation = ImGuizmo::SCALE;
    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(worldTransform.getAsFloatPtr(), matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Translation", matrixTranslation, "%.2f");
    ImGui::InputFloat3("Rotation", matrixRotation, "%.2f");
    ImGui::InputFloat3("Scale", matrixScale, "%.2f");
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &worldTransform.mat[0][0]);
    
    if (currentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
            currentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
            currentGizmoMode = ImGuizmo::WORLD;
    }

	if (ImGui::IsKeyPressed(ImGuiKey_T))
		useSnap = !useSnap;
	ImGui::Checkbox("useSnap", &useSnap);
	ImGui::SameLine();
	switch (currentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		snap = snapConfig.snapTranslation;
		ImGui::InputFloat3("Snap", &snap.x);
		snapConfig.snapTranslation = snap;
		break;
	case ImGuizmo::ROTATE:
		snap = snapConfig.snapRotation;
		ImGui::InputFloat("Angle Snap", &snap.x);
		snapConfig.snapRotation = snap;
		break;
	case ImGuizmo::SCALE:
		snap = snapConfig.snapScale;
		ImGui::InputFloat("Scale Snap", &snap.x);
		snapConfig.snapScale = snap;
		break;
	}
	
}

void Actor::updateImGuizmo()
{
    ImGuizmo::Manipulate(getGame().getRenderer().getViewMatrix().getAsFloatPtr(), getGame().getRenderer().getProjMatrix().getAsFloatPtr(), currentGizmoOperation, currentGizmoMode, &worldTransform.mat[0][0], NULL, useSnap ? snap.getAsFloatPtr() : NULL);
}
#endif

void Actor::load(const rapidjson::Value& data)
{
	if (data.HasMember("Name") && data["Name"].IsString())
	{
		setName(data["Name"].GetString());
	}
	if (data.HasMember("Position") && data["Position"].IsArray())
	{
		const auto& pos = data["Position"];
		Vector3 loadedPos;
		if(pos[0].IsFloat()) loadedPos.x = pos[0].GetFloat();
		if(pos[1].IsFloat()) loadedPos.y = pos[1].GetFloat();
		if(pos[2].IsFloat()) loadedPos.z = pos[2].GetFloat();
		setPosition(loadedPos);
	}
	if (data.HasMember("Rotation") && data["Rotation"].IsArray())
	{
		const auto& rot = data["Rotation"];
		Quaternion loadedRot;
		if(rot[0].IsFloat()) loadedRot.x = rot[0].GetFloat();
		if(rot[1].IsFloat()) loadedRot.y = rot[1].GetFloat();
		if(rot[2].IsFloat()) loadedRot.z = rot[2].GetFloat();
		if(rot[3].IsFloat()) loadedRot.w = rot[3].GetFloat();
		setRotation(loadedRot);
	}
	if (data.HasMember("Scale") && data["Scale"].IsArray())
	{
		const auto& scale = data["Scale"];
		Vector3 loadedScale;
		if(scale[0].IsFloat()) loadedScale.x = scale[0].GetFloat();
		if(scale[1].IsFloat()) loadedScale.y = scale[1].GetFloat();
		if(scale[2].IsFloat()) loadedScale.z = scale[2].GetFloat();
		setScale(loadedScale);
	}
}

rapidjson::Value Actor::save(rapidjson::Document::AllocatorType& allocator)
{
	std::string type = getTypeName();
	rapidjson::Value typeValue(rapidjson::kArrayType);
	typeValue.SetString(type.c_str(), allocator);
	
	rapidjson::Value nameValue(rapidjson::kArrayType);
	std::string name = this->name;
	nameValue.SetString(name.c_str(), allocator);
	
	rapidjson::Value positionToSave(rapidjson::kArrayType);
	positionToSave.PushBack(position.x, allocator);
	positionToSave.PushBack(position.y, allocator);
	positionToSave.PushBack(position.z, allocator);
	
	rapidjson::Value rotationToSave(rapidjson::kArrayType);
	rotationToSave.PushBack(rotation.x, allocator);
	rotationToSave.PushBack(rotation.y, allocator);
	rotationToSave.PushBack(rotation.z, allocator);
	rotationToSave.PushBack(rotation.w, allocator);
	
	rapidjson::Value scaleToSave(rapidjson::kArrayType);
	scaleToSave.PushBack(scale.x, allocator);
	scaleToSave.PushBack(scale.y, allocator);
	scaleToSave.PushBack(scale.z, allocator);
	
	
	rapidjson::Value actorAttributes(rapidjson::kObjectType);
	actorAttributes.AddMember("Type", typeValue, allocator);
	actorAttributes.AddMember("Name", nameValue, allocator);
	actorAttributes.AddMember("Position", positionToSave, allocator);
	actorAttributes.AddMember("Rotation", rotationToSave, allocator);
	actorAttributes.AddMember("Scale",scaleToSave, allocator);
	
	return actorAttributes;

}
