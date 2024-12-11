#include "SaveSystem.h"
#include "Game.h"
#include "Actor.h"
#include "Assets.h"
#include "Component.h"
#include "MeshComponent.h"
#include <prettywriter.h>
#include <stringbuffer.h>
#include <string>
#include <fstream>
#include <filesystem>

using namespace rapidjson;
using namespace std;

void SaveSystem::save()
{
	// Setup document and allocator
	Document document;
	document.SetObject();	
	Document::AllocatorType& allocator = document.GetAllocator();

	// Save all actors attributes
	saveActors(document, allocator);	

	// Parse into JSON
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	document.Accept(writer);

	// Write in an output file
	const string folder = "Ressources";
	const string folder2 = "Saves";
	const string filepath = folder + "/" + folder2 + "/world.json";

	// Créer le dossier si nécessaire
	filesystem::create_directories(folder);
	filesystem::create_directories("Ressources/" + folder2);

	// Ouvrir le fichier pour écriture
	ofstream file(filepath);
	
	//std::ofstream file("world.json");
	file << buffer.GetString();
	file.close();
}

void SaveSystem::saveActors(rapidjson::Document& document, rapidjson::Document::AllocatorType& allocator)
{
	Value actors(rapidjson::kArrayType);
	
	for(auto actor : Game::instance().getActors())
	{
		Value nameValue;
		string name = actor->getName();
		nameValue.SetString(name.c_str(), allocator);

		Value position(rapidjson::kArrayType);
		position.PushBack(actor->getPosition().x, allocator);
		position.PushBack(actor->getPosition().y, allocator);
		position.PushBack(actor->getPosition().z, allocator);

		Value rotation(rapidjson::kArrayType);
		rotation.PushBack(actor->getRotation().x, allocator);
		rotation.PushBack(actor->getRotation().y, allocator);
		rotation.PushBack(actor->getRotation().z, allocator);

		Value scale(rapidjson::kArrayType);
		scale.PushBack(actor->getScale().x, allocator);
		scale.PushBack(actor->getScale().y, allocator);
		scale.PushBack(actor->getScale().z, allocator);

		
		Value actorAttributes(rapidjson::kObjectType);
		actorAttributes.AddMember("Name", nameValue, allocator);
		actorAttributes.AddMember("Position", position, allocator);
		actorAttributes.AddMember("Rotation", rotation, allocator);
		actorAttributes.AddMember("Scale",scale, allocator);

		for(auto component: actor->getComponents())
		{
			if(component->getType() == ComponentType::Mesh)
			{
				if(auto mc = dynamic_cast<MeshComponent*>(component))
				{
					for(auto pair : Assets::meshes)
					{
						if(pair.second.getId() == mc->getMesh()->getId())
						{
							Value meshValue;
							meshValue.SetString(pair.first.c_str(), allocator);
							actorAttributes.AddMember("Mesh",meshValue, allocator);
						}
					}
				}
			}
		}
		
		actors.PushBack(actorAttributes, allocator);
		
	}
	
	document.AddMember("actors", actors, allocator);
}