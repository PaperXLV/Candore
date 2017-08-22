#include "level.h"
#include <iostream>
typedef std::map<std::vector<int>, int>::iterator it_map;



Level::Level(Graphics &graphics) {
	for (int small = level_constants::CORRIDOR_MIN; small < level_constants::CORRIDOR_MAX; small++) {
		this->_possCorridorSizes.push_back(small);
	}
	for (int sizex = level_constants::ROOM_WIDTH_MIN; sizex < level_constants::ROOM_WIDTH_MAX; sizex++) {
		for (int sizey = level_constants::ROOM_HEIGHT_MIN; sizey < level_constants::ROOM_HEIGHT_MAX; sizey++) {
			this->_possRoomSizes.push_back(Vector2(sizex, sizey));
		}
	}

	this->generateLevel(graphics);
}

Level::~Level() {
	//clear data from arrays and stuff
}

void Level::generateLevel(Graphics &graphics) {

	this->dig(13, false, direction::RIGHT);

	this->emptyRooms();
	this->fillOutside();
	this->createBouncePads();
	this->createLights();
	this->createLevel(graphics);
	this->identifyCorners();
	this->addCollidersToLights();

}

void Level::dig(int turns, bool spawnable, direction::Direction dir) {
	Digger digger = Digger(Vector2(100, 100));

	digger.currentdirection = dir;
	if (digger.currentdirection = direction::NONE) {
		digger.currentdirection = (rand() % 100 > 50) ? direction::RIGHT : direction::LEFT;
	}

	this->_start = Vector2((digger.currentdirection == direction::RIGHT) ? 100 : 99, 99);

	int spawnNew = 0;
	//Run $turns turns of diggah
	for (int digs = 0; digs < turns; digs++) {
		
		direction::Direction prevDir = digger.currentdirection;

		if (rand() % 100 < digger.down) {
			//possible we get errors here if there arent any rooms but there should be rooms so something would be fucked			
			//bug to do with how blocks draw and such
			if (digger.currentdirection == direction::RIGHT) {
				digger.position.x -= 2;
			}
			int randIndex = rand() % this->_possCorridorSizes.size();
			Room newRoom = Room(Rectangle(digger.position.x, digger.position.y, 2, this->_possCorridorSizes[randIndex]), true);
			this->_rooms.push_back(newRoom);
			digger.position = Vector2(newRoom.getRect().getCenterX(), newRoom.getRect().getBottom());
			digger.down = 0;

			prevDir = direction::DOWN;
			digger.currentdirection = (rand() % 100 > 50) ? direction::RIGHT : direction::LEFT;
		}
		else {
			digger.down += 20;
		}

		if (digger.currentdirection == prevDir) {
			(rand() % 100 > 50) ? digger.position.y += 1 : digger.position.y -= 1;
		}

		if (rand() % 100 < spawnNew) {
				direction::Direction corridorDir = (rand() % 100 > 50) ? direction::DOWN : direction::UP;

				int randIndex = rand() % this->_possCorridorSizes.size();
				
				int otherrandIndex = rand() % this->_possRoomSizes.size();				
				
				Vector2 centerCurrRoom = Vector2(this->_rooms[this->_rooms.size() - 1].getRect().getCenterX(), this->_rooms[this->_rooms.size() - 1].getRect().getBottom());

				Room newRoom = Room(Rectangle(centerCurrRoom.x - (std::round(this->_possRoomSizes[otherrandIndex].x / 2)), 
					(corridorDir == direction::DOWN) ? centerCurrRoom.y + this->_possCorridorSizes[randIndex] - this->_possRoomSizes[otherrandIndex].y : 
					centerCurrRoom.y - this->_possCorridorSizes[randIndex] - this->_possRoomSizes[otherrandIndex].y, this->_possRoomSizes[otherrandIndex].x,
					this->_possRoomSizes[otherrandIndex].y), false);
				if (!checkoverlap(newRoom)) {
					this->_rooms.push_back(Room(Rectangle(centerCurrRoom.x, (corridorDir == direction::DOWN) ? centerCurrRoom.y - 1: centerCurrRoom.y - this->_possCorridorSizes[randIndex],
						2, this->_possCorridorSizes[randIndex]), true));
					this->_rooms.push_back(newRoom);
					spawnNew = 0;
				}
			}
			else {
				spawnNew += 20;
			}


		int randIndex = rand() % this->_possRoomSizes.size();
		Room newRoom = Room(Rectangle(
			(digger.currentdirection == direction::RIGHT) ? digger.position.x : digger.position.x - (this->_possRoomSizes[randIndex].x),
			digger.position.y - (this->_possRoomSizes[randIndex].y), this->_possRoomSizes[randIndex].x, this->_possRoomSizes[randIndex].y), false);
		this->_rooms.push_back(newRoom);
		digger.position = Vector2((digger.currentdirection == direction::RIGHT) ? newRoom.getRect().getRight() : newRoom.getRect().getLeft(), newRoom.getRect().getBottom());
	}
}

bool Level::checkoverlap(Room &possible) {
	Collider newroom = Collider(Vector2(possible.getRect().getLeft() - 2, possible.getRect().getTop() - 2), Rectangle(possible.getRect().getLeft() - 2, possible.getRect().getTop() - 2, possible.getRect().getWidth() + 4, possible.getRect().getHeight() + 4));
	int collisions = 0;
	for (int i = 0; i < this->_rooms.size(); i++) {
		Collider roomcollider = Collider(Vector2(this->_rooms[i].getRect().getLeft() - 2, this->_rooms[i].getRect().getTop() - 2), Rectangle(this->_rooms[i].getRect().getLeft() - 2, this->_rooms[i].getRect().getTop() - 2, this->_rooms[i].getRect().getWidth() + 4, this->_rooms[i].getRect().getHeight() + 4));
		if (newroom.is_colliding(roomcollider)) {
			collisions++;
			return (collisions != 0);
		}
	}
	return (collisions != 0);
}

bool Level::checkcorridor(Corridor &possible) {
	int collisions = 0;
	for (int i = 0; i < this->_rooms.size(); i++) {
		Rectangle currentRect = this->_rooms[i].getRect();
		//Rectangle currentRect = Rectangle(this->_rooms[i].getRect().getLeft() - 2, this->_rooms[i].getRect().getTop() - 2, this->_rooms[i].getRect().getWidth() + 4, this->_rooms[i].getRect().getHeight() + 4);
		if (possible.is_colliding(currentRect)) {
			collisions++;
			return (collisions != 0);
		}
	}
	return (collisions != 0);
}

void Level::emptyRooms() {
	for (int i = 0; i < this->_rooms.size(); i++) {
		Rectangle currentRect = this->_rooms[i].getRect();
		for (int x = currentRect.getLeft(); x < currentRect.getRight(); x++) {
			for (int y = currentRect.getTop(); y < currentRect.getBottom(); y++) {
				std::vector<int> coord = { x, y };
				this->_map[coord] = 0;
			}
		}
	}
	for (int i = 0; i < this->_corridors.size(); i++) {
		Rectangle currentRect = this->_corridors[i].getRect();
		for (int x = currentRect.getLeft(); x < currentRect.getRight(); x++) {
			for (int y = currentRect.getTop(); y < currentRect.getBottom(); y++) {
				std::vector<int> coord = { x, y };
				this->_map[coord] = 0;
			}
		}
	}
}

void Level::fillOutside() {
	for (int i = 0; i < this->_rooms.size(); i++) {
		Rectangle currentRect = this->_rooms[i].getRect();

		//Top Side
		int y = currentRect.getTop() - 1;
		for (int x = currentRect.getLeft() - 1; x < currentRect.getRight() + 1; x++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}

		//Bottom Side
		y = currentRect.getBottom();
		for (int x = currentRect.getLeft() - 1; x < currentRect.getRight() + 1; x++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}

		//Left Side
		int x = currentRect.getLeft() - 1;
		for (y = currentRect.getTop() - 1; y < currentRect.getBottom(); y++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}

		//Right Side
		x = currentRect.getRight();
		for (y = currentRect.getTop() - 1; y < currentRect.getBottom(); y++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}
	}

	//Corridors
	for (int i = 0; i < this->_corridors.size(); i++) {
		Rectangle currentRect = this->_corridors[i].getRect();

		//Top Side
		int y = currentRect.getTop() - 1;
		for (int x = currentRect.getLeft() - 1; x < currentRect.getRight(); x++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}

		//Bottom Side
		y = currentRect.getBottom();
		for (int x = currentRect.getLeft() - 1; x < currentRect.getRight(); x++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}

		//Left Side
		int x = currentRect.getLeft() - 1;
		for (y = currentRect.getTop() - 1; y < currentRect.getBottom(); y++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}

		//Right Side
		x = currentRect.getRight();
		for (y = currentRect.getTop() - 1; y < currentRect.getBottom(); y++) {
			std::vector<int> coord = { x, y };
			this->addBlock(coord);
		}
	}
}

void Level::addBlock(std::vector<int> coord) {
	//checks for if data is uninit and that it isn't 0 already :)
	//Breakin the program for the good of the program, thats what I'm talkin aboot
	try {
		if (!this->_map.at(coord) == 0) {
			this->_map[coord] = 1;
		}
	}
	catch (const std::out_of_range& oor) {
		this->_map[coord] = 1;
	}
}

//TODO::Optimize createBouncePads and createLights by combining function, essentially relies on one bool check, and go into correct based on corridor
void Level::createBouncePads() {
	for (int i = 0; i < this->_rooms.size(); i++) {
		if (this->_rooms[i].getCooridor()) {
			std::vector<int> possXCoord;
			int y = this->_rooms[i].getRect().getBottom();
			while (possXCoord.size() == 0) {
				for (int x = this->_rooms[i].getRect().getLeft(); x < this->_rooms[i].getRect().getRight(); x++) {
					std::vector<int> coord{ x, y };
					try {
						if (this->_map.at(coord) == 1) {
							possXCoord.push_back(x);
						}
					}
					catch (const std::out_of_range& oor) {
						std::cout << "we got out of range on bounce pad, weird\n";
						//this->_map[coord] = 2;
					}
				}
				y++;
			}

			if (possXCoord.size() > 0) {
				int randIndex = rand() % possXCoord.size();
				std::vector<int> coord{ possXCoord[randIndex], y - 1};
				try {
					this->_map[coord] = 2;
				}
				catch (const std::out_of_range& oor) {
					std::cout << "need to write handler for bounce pads\n";
				}
			}
		}
	}
}

void Level::createLights() {
	for (int i = 0; i < this->_rooms.size(); i++) {
		Room current = this->_rooms[i];
		if (!current.getCooridor()) {
			if (current.getRect().getWidth() > level_constants::DIST_BETWEEN_LIGHTS) {
				int numberoflights = std::floor(current.getRect().getWidth() / level_constants::DIST_BETWEEN_LIGHTS);
				int remainingBlocks = current.getRect().getWidth() % level_constants::DIST_BETWEEN_LIGHTS;
				int blocksOnStart = remainingBlocks / numberoflights;
				for (int lightcount = 0; lightcount < numberoflights; lightcount++) {
					std::vector<int> coord{ current.getRect().getLeft() + blocksOnStart + (level_constants::DIST_BETWEEN_LIGHTS * lightcount), current.getRect().getTop() };
					std::vector<int> above{ current.getRect().getLeft() + blocksOnStart + (level_constants::DIST_BETWEEN_LIGHTS * lightcount), current.getRect().getTop() - 1 };
					try {
						if (this->_map[above] == 1) {
							this->_map[coord] = 3;
						}
					}
					catch (const std::out_of_range& oor) {
						std::cout << "need to write handler for lights\n";
					}
				}
			}
			else {
				if (rand() % 100 > 50) {
					std::vector<int> coord{ (current.getRect().getLeft() + current.getRect().getRight()) / 2, current.getRect().getTop() };
					std::vector<int> above{ (current.getRect().getLeft() + current.getRect().getRight()) / 2, current.getRect().getTop() - 1 };

					try {
						if (this->_map[above] == 1) {
							this->_map[coord] = 3;
						}
					}
					catch (const std::out_of_range& oor) {
						std::cout << "need to write handler for lights\n";
					}
				}
			}
		}
	}
}

void Level::createRow(int y) {
	for (int x = 0; x < level_constants::LEVEL_WIDTH; x++) {
		std::vector<int> coord = { x, y };
		if (this->_map[coord] == 1) {
			bool block = true;
			int next = 0;
			std::vector<int> combine;
			combine.push_back(x);
			while (block) {
				next++;
				std::vector<int> nextcoord;
				nextcoord.push_back(x + next); nextcoord.push_back(y);
				if (this->_map[nextcoord] == 1) {
					combine.push_back(nextcoord[0]);
				}
				else {
					block = false;
				}
			}
			Collider newCollider = Collider(Vector2(x, y), Rectangle(x, y, block_constants::width * next, block_constants::height));
			for (int j = 0; j < next; j++) {
				std::vector<int> nextcoord = { x + j, y };
				this->_blockmap[nextcoord].setCollider(newCollider);
			}
			this->_blockmap[coord].setCollider(newCollider);
			this->_colliders.push_back(newCollider);
			x += next;
		}
	}
}

void Level::createCollumn(int x) {
	for (int y = 0; y < level_constants::LEVEL_HEIGHT; y++) {
		std::vector<int> coord = { x, y };
		if (this->_map[coord] == 1) {
			bool block = true;
			int next = 0;
			std::vector<int> combine;
			combine.push_back(y);
			while (block) {
				next++;
				std::vector<int> nextcoord;
				nextcoord.push_back(x); nextcoord.push_back(y + next);
				if (this->_map[nextcoord] == 1) {
					combine.push_back(nextcoord[0]);
				}
				else {
					block = false;
				}
			}
			Collider newCollider = Collider(Vector2(x, y), Rectangle(x, y, block_constants::width, block_constants::height * next));
			for (int j = 0; j < next; j++) {
				std::vector<int> nextcoord = { x, y + j };
				this->_blockmap[nextcoord].setCollider(newCollider);
			}
			this->_blockmap[coord].setCollider(newCollider);
			this->_colliders.push_back(newCollider);
			y += next;
		}
	}
}

void Level::createLevel(Graphics &graphics) {
	for (it_map i = this->_map.begin(); i != this->_map.end(); i++) {
		if (i->second == 1) {
			std::vector<int> coord = i->first;
			Block newBlock = Block(graphics, "Assets/sprites/stoic/Stone_Brick.png", 0, 0, 32, 32, Vector2(coord[0] * level_constants::BLOCK_TO_PIXEL_SCALE, coord[1] * level_constants::BLOCK_TO_PIXEL_SCALE));
			this->_blocks.push_back(newBlock);
			this->_blockmap[coord] = newBlock;
			this->_colliders.push_back(newBlock.getCollider());
		}
		else if (i->second == 2) {
			std::vector<int> coord = i->first;
			BouncePad newBouncePad = BouncePad(graphics, Vector2(coord[0] * level_constants::BLOCK_TO_PIXEL_SCALE, coord[1] * level_constants::BLOCK_TO_PIXEL_SCALE));
			this->_bouncePads.push_back(newBouncePad);
			this->_bouncemap[coord] = newBouncePad;
			this->_colliders.push_back(newBouncePad.getCollider());
		}
		else if (i->second == 3) {
			std::vector<int> coord = i->first;
			Light newLight = Light(graphics, Vector2(coord[0] * level_constants::BLOCK_TO_PIXEL_SCALE, coord[1] * level_constants::BLOCK_TO_PIXEL_SCALE));
			this->_lights.push_back(newLight);
			this->_lightmap[coord] = newLight;
		}
	}
	this->_start = Vector2(this->_rooms[0].getRect().getCenterX()* level_constants::BLOCK_TO_PIXEL_SCALE, (this->_rooms[0].getRect().getBottom() * level_constants::BLOCK_TO_PIXEL_SCALE) - 16);


	//this->_start = Vector2(this->_start.x * level_constants::BLOCK_TO_PIXEL_SCALE, this->_start.y * level_constants::BLOCK_TO_PIXEL_SCALE);
	this->_end = Vector2(this->_end.x * level_constants::BLOCK_TO_PIXEL_SCALE, this->_start.y * level_constants::BLOCK_TO_PIXEL_SCALE);

	//create doors
	Door startDoor = Door(graphics, "Assets/sprites/stoic/Door.png", 0, 0, 32, 64, Vector2(this->_start.x - 8, this->_start.y - 48));
	Door endDoor = Door(graphics, "Assets/sprites/stoic/Door.png", 0, 0, 32, 64, Vector2(this->_end.x, this->_end.y - 64));

	this->_doors.push_back(startDoor); this->_doors.push_back(endDoor);
}

void Level::identifyCorners() {
	for (int i = 0; i < this->_colliders.size(); i++) {
		this->_colliders[i].generateCorners(this->_map);
	}
}

void Level::addCollidersToLights() {
	for (int i = 0; i < this->_colliders.size(); i++) { 
		for (int j = 0; j < this->_lights.size(); j++) {
			Vector2 dist = Vector2(this->_colliders[i].getPosition().getCenterX() - this->_lights[j].getCenter().x,
				this->_colliders[i].getPosition().getCenterY() - this->_lights[j].getCenter().y);
			float normalizedSq = (dist.x * dist.x) + (dist.y * dist.y);
			if (normalizedSq < level_constants::DIST_OF_LIGHT * level_constants::DIST_OF_LIGHT) {
				this->_lights[j].addCollider(this->_colliders[i]);
			}
		}
	}
	for (int i = 0; i < this->_lights.size(); i++) {
		this->_lights[i].initializeRays();
	}
}

void Level::spawnEnemies() {
	//add enemy spawning code
}

std::vector<Polygon> Level::getLightPolygons(Vector2 offset) {
	std::vector<Polygon> polygons;
	for (int i = 0; i < this->_lights.size(); i++) {
		polygons.push_back(this->_lights[i].generateLightPolygon(offset));
	}
	return polygons;
}

std::vector<Collider> Level::getNeighborColliders(int posX, int posY) {
	std::vector<int> coords;
	coords.push_back(posX); coords.push_back(posY);

	std::vector<Collider> colliders;
	//check the blocks in a 3x3 radius around point
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			//find collider by coord
			std::vector<int> coord = { x + posX, y + posY };
			if (this->_map[coord] == 1) {
				Collider current = this->_blockmap[coord].getCollider();
				colliders.push_back(current);
			}
			else if (this->_map[coord] == 2) {
				Collider current = this->_bouncemap[coord].getCollider();
				colliders.push_back(current);
			}
		}
	}

	return colliders;
}

std::vector<Collider> Level::getNeighborBounceColliders(int posX, int posY) {
	std::vector<int> coords{ posX, posY };

	std::vector<Collider> colliders;

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			std::vector<int> coord = { x + posX, y + posY };
			if (this->_map[coord] == 2) {
				Collider current = this->_bouncemap[coord].getBounceCollider();
				colliders.push_back(current);
			}
		}
	}

	return colliders;
}

inline std::vector<direction::Direction> Level::getOtherDirections(direction::Direction current) {
	std::vector<direction::Direction> remaining;
	if (current == direction::RIGHT) {
		remaining = { direction::LEFT, direction::DOWN, direction::UP };
		return remaining;
	}
	else if (current == direction::LEFT) {
		remaining = { direction::RIGHT, direction::DOWN, direction::UP };
		return remaining;
	}
	else if (current == direction::DOWN) {
		remaining = { direction::RIGHT, direction::LEFT, direction::UP };
		return remaining;
	}
	else if (current == direction::UP) {
		remaining = { direction::RIGHT, direction::LEFT, direction::DOWN };
		return remaining;
	}
	else {
		return remaining;
	}
}

void Level::update(int elapsedTime) {	
	for (int i = 0; i < this->_lights.size(); i++) {
		this->_lights[i].update(elapsedTime);
	}
	for (int i = 0; i < this->_bouncePads.size(); i++) {
		this->_bouncePads[i].update(elapsedTime);
	}
	for (int i = 0; i < this->_blocks.size(); i++) {
		this->_blocks[i].update(elapsedTime);
	}
	for (int i = 0; i < this->_doors.size(); i++) {
		this->_doors[i].update(elapsedTime);
	}
}

void Level::draw(Graphics &graphics, Vector2 offset) {
	for (int i = 0; i < this->_lights.size(); i++) {
		this->_lights[i].draw(graphics, offset);
	}
	for (int i = 0; i < this->_bouncePads.size(); i++) {
		this->_bouncePads[i].draw(graphics, offset);
	}
	for (int i = 0; i < this->_blocks.size(); i++) {
		this->_blocks[i].draw(graphics, offset);
	}
	for (int i = 0; i < this->_doors.size(); i++) {
		this->_doors[i].draw(graphics, offset);
	}
}