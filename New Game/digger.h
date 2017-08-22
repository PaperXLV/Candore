#pragma once
#ifndef DIGGER_H
#define DIGGER_H

#include "globals.h"
#include <random>

struct Digger {
	Digger() {}
	Digger(Vector2 spawn) :
		position(spawn)
	{
		this->currentdirection = static_cast<direction::Direction>(rand() % 3);
	}



	int directionchange = 10;
	int down = 0;

	direction::Direction currentdirection;
	Vector2 position;
};




#endif DIGGER_H