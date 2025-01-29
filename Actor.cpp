#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "Component.h"
#include "imgui.h"
#include "Maths.h"

Actor::Actor() :
	state(Actor::ActorState::Active),
	position(Vector3::zero),
	scale(Vector3(1.0f, 1.0f, 1.0f)),
	rotation(Quaternion::identity),
	mustRecomputeWorldTransform(true),
	game(Game::instance()),
	name("Unnamed")
#ifdef _DEBUG
	,isScaleLocked(false)
#endif
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
void Actor::updateImGUIOutliner()
{
	ImGui::Text(name.c_str());
				
	Vector3 currentPosition = getPosition();
	Vector3 uiPosition = currentPosition;
					
	if (ImGui::DragFloat3("Position", &uiPosition.x, 1.0f)) {
		if (uiPosition != currentPosition) {
			setPosition(uiPosition);
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
						
			setRotation(rot);
		}
	}

	// Scale
				
	Vector3 currentScale = getScale();
	Vector3 uiScale = currentScale;
				
	if (ImGui::DragFloat3("Scale", &uiScale.x, 0.1f)) {
		if (uiScale != currentScale) {
			if (isScaleLocked)
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
			setScale(uiScale);
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("Lock", &isScaleLocked);
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
