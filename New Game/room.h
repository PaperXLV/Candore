#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "rectangle.h"

class Room {
public:
	Room() {}
	Room(Rectangle roomrect, bool isCorridor) :
		_roomrect(roomrect),
		_corridor(isCorridor)
	{}
	~Room() {}

	const Rectangle const getRect() { return this->_roomrect; }

	//if it's a corridor, we need bouncepad on the bottom
	const inline bool getCooridor() { return this->_corridor; }


private:
	Rectangle _roomrect;

	bool _corridor;

};



#endif ROOM_H