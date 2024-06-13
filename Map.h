#pragma once
#include "Actor.h"
#include "Quaternion.h"
#include <vector>

class Map
{
public:
	explicit Map(const std::vector<std::vector<int>>& map);
	~Map();
	Map(const Map&) = delete;
	Map& operator =(const Map&) = delete;

	void initializeMap();
	void setMap(const std::vector<std::vector<int>>& map);
	std::vector<std::vector<int>> getMap() { return map;}


private:
	std::vector<std::vector<int>> map;
	std::vector<Actor*> actors;
	Quaternion q, q2; // Rotations Z axis
	static const int sizeActors;
	static const int wallHeight;
};
