#include "SaveSystem.h"
#include "Game.h"
#include "Actor.h"
#include "ActorFactory.h"
#include "Assets.h"
#include "Component.h"
#include "MeshComponent.h"
#include <prettywriter.h>
#include <stringbuffer.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <document.h>
#include <tchar.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <sstream>

using namespace rapidjson;
using namespace std;

static const std::filesystem::path initialPath = std::filesystem::current_path();
static std::filesystem::path currentPath;

void SaveSystem::saveAs()
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = _T("");

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = _T("JSON Files(*.json)\0*.json\0");
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = _T("json");

	if (GetSaveFileName(&ofn)) {
		SDL_Log( "File saved");
	} else {
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "Save canceled");
	}
	
	currentPath = ofn.lpstrFile;
	saveInFile(ofn.lpstrFile);
}

void SaveSystem::save()
{
	if(currentPath.empty()) saveAs();
	else saveInFile(currentPath);
}

void SaveSystem::saveInFile(filesystem::path path)
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


	// Ouvrir le fichier pour écriture
	ofstream file(path);
	
	//std::ofstream file("world.json");
	file << buffer.GetString();
	file.close();
}

void SaveSystem::saveActors(rapidjson::Document& document, rapidjson::Document::AllocatorType& allocator)
{
	Value actors(rapidjson::kArrayType);
	
	for(auto actor : Game::instance().getActors())
	{
		string type = actor->getTypeName();
		if(type == "ViewportActor")
		{
			continue;
		}
		rapidjson::Value actorAttributes(rapidjson::kObjectType);
		actorAttributes = actor->save(allocator);
		for(auto component: actor->getComponents())
		{
			component->save(actorAttributes, allocator);
		}
		
		actors.PushBack(actorAttributes, allocator);
		
	}
	
	document.AddMember("Actors", actors, allocator);
}

std::wstring SaveSystem::openFilePath()
{
	TCHAR szFile[MAX_PATH] = _T(""); 

	// Initialisation de la structure OPENFILENAME
	OPENFILENAME ofn = {0}; // Zero-initialisation pour éviter des valeurs non définies
	ofn.lStructSize = sizeof(OPENFILENAME); // Taille de la structure
	ofn.hwndOwner = NULL; // Fenêtre parente (NULL si aucune)
	ofn.lpstrFile = szFile; // Pointeur vers le buffer pour le chemin du fichier
	ofn.nMaxFile = MAX_PATH; // Taille du buffer
	ofn.lpstrFilter = _T("JSON Files\0*.json\0"); // Filtres
	ofn.nFilterIndex = 1; // Index du filtre sélectionné par défaut
	ofn.lpstrFileTitle = NULL; // Titre du fichier (non utilisé ici)
	ofn.nMaxFileTitle = 0; // Taille du buffer pour lpstrFileTitle
	ofn.lpstrInitialDir = NULL; // Répertoire initial (NULL pour le dernier utilisé)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Options (par ex., chemin doit exister)
	
	if (GetOpenFileName(&ofn)) {
		std::wstring wFilePath(ofn.lpstrFile);
		return wFilePath;
	} else {
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "No file selected.");
		return std::wstring();
	}
}

void SaveSystem::loadFile()
{
	std::wstring filepath = openFilePath();
	load(filepath);
}

void SaveSystem::loadFirstFile()
{
#ifdef _DEBUG
	const filesystem::path folder = "Ressources";
	const filesystem::path savesFolder = folder / "Saves";
	const filesystem::path filepath = savesFolder / "editorDefaultMap.json";
	if (filesystem::exists(filepath))
	{
		load(filepath);
	}
	else
	{
		loadFile();
	}
#else
	const filesystem::path folder = "Ressources";
	const filesystem::path savesFolder = folder / "Saves";
	const filesystem::path filepath = savesFolder / "editorDefaultMap.json";
	if (filesystem::exists(filepath))
	{
		load(filepath);
	}
	else
	{
		loadFile();
	}
#endif
}

void SaveSystem::load(std::wstring filepath)
{
	// Check if the file exists
	if (!filesystem::exists(filepath))
	{
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "File does not exist !");
		return;
	}
	
	ifstream file(filepath);
	if (!file.is_open())
	{
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "Could not open file !");
		return;
	}

	// Read the entire file content into a string
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	Document document;
	if (document.Parse(buffer.str().c_str()).HasParseError())
	{
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "Failed to parse JSON file !");
		return;
	}
	
	if (!document.IsObject())
	{
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "Invalid JSON structure: Root is not an object");
		return;
	}

	// Clear Actors
	Game::instance().clearActors();
	
	// Load actors or other game objects
	loadActors(document);

	currentPath = filepath;
}

void SaveSystem::loadActors(rapidjson::Document& document)
{
	if (!document.HasMember("Actors") || !document["Actors"].IsArray())
	{
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "Actors data missing or not an array in the JSON file.");
		return;
	}

	const auto& actorsArray = document["Actors"];
	for (const auto& actorData : actorsArray.GetArray())
	{
		if (actorData.HasMember("Type") && actorData["Type"].IsString()) {
			std::string type = actorData["Type"].GetString();

			auto actor = ActorFactory::getInstance().create(type);
			if (actor) {
				actor->load(actorData);
				for(auto component : actor->getComponents())
				{
					component->load(actorData);
				}
			} else {
				SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_SYSTEM, "Unknown actor type");
			}
		}
	}
}

void SaveSystem::reloadCurrentMap()
{
	if(!currentPath.empty())
	{
		load(currentPath);
	}
}

