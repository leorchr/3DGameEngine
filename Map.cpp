#include "Map.h"
#include "Assets.h"
#include "CubeActor.h"
#include "DoorActor.h"
#include "Game.h"
#include "KeyActor.h"
#include "LockedDoorActor.h"
#include "MovingPlatformActor.h"
#include "Vector3.h"

const int Map::sizeActors = 500;
const int Map::wallHeight = 150;

Map::Map(const std::vector<std::vector<int>>& map) :
	map(map)
{
	q = Quaternion(Vector3::unitY, Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi));
	q2 = q;
	q2 = Quaternion::concatenate(q2, Quaternion(Vector3::unitZ, Maths::piOver2));
}

Map::~Map()
{
	for (auto a : actors)
	{
		a->setState(Actor::ActorState::Dead);
	}
}

void Map::initializeMap()
{
	// Place all actors depending on the index of the map
	for (size_t i = 0; i < map.size(); i++)
	{
		const int iPos = static_cast<size_t>(i);
		std::vector<int> invertMap = map[i];
		std::reverse(invertMap.begin(), invertMap.end());
		
		for (size_t y = 0; y < map[i].size(); y++)
		{
			const int yPos = static_cast<size_t>(y);
			if (invertMap[y] == 1) {										// Walls
				Actor* wall = new CubeActor();
				wall->setScale(Vector3(sizeActors, sizeActors, sizeActors));
				wall->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, wallHeight));
				wall->setRotation(q);
				actors.push_back(wall);
			}
			else if (invertMap[y] == 2) {                                   // Player start position
				Game::instance().getPlayer()->setPosition(Vector3(sizeActors * iPos,sizeActors * yPos, 0));
			}
			else if (invertMap[y] == 3) {                                   // Player objective
				Game::instance().getPlayer()->setObjectivePos(Vector3(sizeActors * iPos,sizeActors * yPos, 0));
			}
			else if (invertMap[y] == 4) {                                   // Simple door
				Actor* door = new DoorActor();
				door->setScale(Vector3(sizeActors, sizeActors, sizeActors));
				door->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, wallHeight));
				door->setRotation(q);
				actors.push_back(door);
			}
			else if (invertMap[y] == 5) {                                   // Yellow locked door
				const auto lockedDoor = new LockedDoorActor();
				lockedDoor->setKey(0);
				lockedDoor->setScale(Vector3(sizeActors, sizeActors, sizeActors));
				lockedDoor->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, wallHeight));
				lockedDoor->setRotation(q);
				actors.push_back(lockedDoor);
			}
			else if (invertMap[y] == 6) {                                   // Red locked door
				const auto lockedDoor = new LockedDoorActor();
				lockedDoor->setKey(1);
				lockedDoor->setScale(Vector3(sizeActors, sizeActors, sizeActors));
				lockedDoor->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, wallHeight));
				lockedDoor->setRotation(q);
				actors.push_back(lockedDoor);
			}
			else if (invertMap[y] == 7) {                                   // Yellow key
				const auto key = new KeyActor();
				key->setKey(0);
				key->setScale(Vector3(30, 30, 30));
				key->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, -50));
				key->setRotation(q);
				actors.push_back(key);
			}
			else if (invertMap[y] == 8) {                                   // Red key
				const auto key = new KeyActor();
				key->setKey(1);
				key->setScale(Vector3(30, 30, 30));
				key->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, -50));
				key->setRotation(q);
				actors.push_back(key);
			}
			else if (invertMap[y] == 9) {                                   // Moving platform x-axis
				const auto mp = new MovingPlatformActor(false);
				mp->setStart(Vector3(sizeActors * iPos, sizeActors * yPos, -101));
				actors.push_back(mp);
			}
			else if (invertMap[y] == 10) {                                  // Moving platform y-axis
				const auto mp = new MovingPlatformActor(true);
				mp->setStart(Vector3(sizeActors * iPos, sizeActors * yPos, -101));
				actors.push_back(mp);
			}
			else if (invertMap[y] == 11) {                                  // Traps
				const auto wall = new PlaneActor();
				wall->getMesh()->setTextureIndex(2);
				wall->setScale(Vector3(5.0f, 5.0f, 5.0f));
				wall->setRotation(q);
				wall->setPosition(Vector3(sizeActors * iPos + 250, sizeActors * yPos, -351));

				const auto wall2 = new PlaneActor();
				wall2->getMesh()->setTextureIndex(2);
				wall2->setScale(Vector3(5.0f, 5.0f, 5.0f));
				wall2->setRotation(q2);
				wall2->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos - 250, -351));

				const auto wall3 = new PlaneActor();
				wall3->getMesh()->setTextureIndex(2);
				wall3->setScale(Vector3(5.0f, 5.0f, 5.0f));
				wall3->setRotation(q2);
				wall3->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos + 250, -351));

				const auto wall4 = new PlaneActor();
				wall4->getMesh()->setTextureIndex(2);
				wall4->setScale(Vector3(5.0f, 5.0f, 5.0f));
				wall4->setRotation(q);
				wall4->setPosition(Vector3(sizeActors * iPos - 250, sizeActors * yPos, -351));
				
				const auto trap = new PlaneActor();
				trap->getMesh()->setTextureIndex(3);
				trap->setScale(Vector3(5.0f, 5.0f, 5.0f));
				trap->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, -601));

				actors.push_back(wall);
				actors.push_back(wall2);
				actors.push_back(wall3);
				actors.push_back(wall4);
				actors.push_back(trap);
			}
			if (invertMap[y] <= 8) {										// Place plane grounds everywhere except traps and moving platforms
				const auto ground = new PlaneActor();
				ground->getMesh()->setTextureIndex(1);
				ground->setScale(Vector3(5.0f, 5.0f, 5.0f));
				ground->setPosition(Vector3(sizeActors * iPos, sizeActors * yPos, -101));
				actors.push_back(ground);
			}
		}
	}
}

void Map::setMap(const std::vector<std::vector<int>>& map)
{
	this->map = map;
}