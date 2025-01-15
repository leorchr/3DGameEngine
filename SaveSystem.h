#pragma once
#include <document.h>
#include <string>

class SaveSystem
{
public:
	SaveSystem();
	SaveSystem(const SaveSystem&) = delete;
	SaveSystem& operator=(const SaveSystem&) = delete;

	static void save();
	static void saveActors(rapidjson::Document& document, rapidjson::Document::AllocatorType& allocator);
	static std::wstring openFilePath();
	static void load();
	static void loadActors(rapidjson::Document& document);
};