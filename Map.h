#pragma once
#include "Quaternion.h"
#include <vector>

class Map
{
public:
	explicit Map(const std::vector<std::vector<int>>& map);
	Map(const Map&) = delete;
	Map& operator =(const Map&) = delete;

	void initializeMap() const;
	void setMap(const std::vector<std::vector<int>>& map);
	std::vector<std::vector<int>> getMap() { return map;}


private:
	std::vector<std::vector<int>> map;
	Quaternion q, q2; // Rotations Z axis
	static const int sizeActors;
	static const int wallHeight;
};
