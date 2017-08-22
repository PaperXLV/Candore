#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "animatedsprite.h"
#include "room.h"
#include "block.h"
#include "bouncepad.h"
#include "light.h"
#include "door.h"
#include "digger.h"
#include "corridor.h"
#include <map>




class Level {
public:
	Level() {}
	Level(Graphics &graphics);
	~Level();

	void generateLevel(Graphics &graphics);
	void emptyRooms();
	void fillOutside();
	void createBouncePads();
	void createLights();
	void identifyCorners();
	void addCollidersToLights();
	//add objects to level

	void addBlock(std::vector<int> coord);
	void createRow(int y);
	void createCollumn(int x);

	void dig(int turns, bool spawnable, direction::Direction dir);
	bool checkoverlap(Room &possible);
	bool checkcorridor(Corridor &possible);

	std::vector<Polygon> getLightPolygons(Vector2 offset);

	void spawnEnemies();

	//methods
	std::vector<Collider> getNeighborColliders(int posX, int posY);
	std::vector<Collider> getNeighborBounceColliders(int posX, int posY);
	const Vector2 getSpawnPosition() const { return this->_start; }
	inline std::vector<direction::Direction> getOtherDirections(direction::Direction current);


	void createLevel(Graphics &graphics);

	void draw(Graphics &graphics, Vector2 offset);
	void update(int elapsedTime);

private:
	std::vector<Vector2> _coords;
	std::map<std::vector<int>, int> _map;
	std::map<std::vector<int>, Block> _blockmap;
	std::map<std::vector<int>, BouncePad> _bouncemap;
	std::map<std::vector<int>, Light> _lightmap;

	Room _startRoom, _endRoom;

	std::vector<Room> _rooms;
	std::vector<Corridor> _corridors;
	std::vector<Block> _blocks;
	std::vector<BouncePad> _bouncePads;
	std::vector<Light> _lights;
	std::vector<Collider> _colliders;
	std::vector<Door> _doors;

	std::vector<Vector2> _possRoomSizes;
	std::vector<int> _possCorridorSizes;


	//more arrays holding diff data
	Vector2 _start, _end;

};


#endif LEVEL_H