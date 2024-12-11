#include <document.h>

#pragma once

class SaveSystem
{
public:
	SaveSystem();
	SaveSystem(const SaveSystem&) = delete;
	SaveSystem& operator=(const SaveSystem&) = delete;

	static void save();
	static void saveActors(rapidjson::Document& document, rapidjson::Document::AllocatorType& allocator);
};