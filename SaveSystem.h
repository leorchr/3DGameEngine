#pragma once
#include <document.h>
#include <string>

namespace std::filesystem {
	class path;
}

class SaveSystem
{
public:
	SaveSystem() = default;
	SaveSystem(const SaveSystem&) = delete;
	SaveSystem& operator=(const SaveSystem&) = delete;

	static void saveAs();
	static void save();
	static void saveInFile(std::filesystem::path path);
	static void saveActors(rapidjson::Document& document, rapidjson::Document::AllocatorType& allocator);
	static std::wstring openFilePath();
	static void loadFile();
	static void loadFirstFile();
	static void load(std::wstring filepath);
	static void loadActors(rapidjson::Document& document);
};