#pragma once
#ifndef CORRIDOR_H
#define CORRIDOR_H

#include "globals.h"
#include "rectangle.h"

class Corridor {
public:
	Corridor() {}
	Corridor(Vector2 position, direction::Direction dir, int dist) :
		_position(position),
		_direction(dir),
		_dist(dist)
	{
		if (this->_direction == direction::RIGHT) {
			this->_rect = Rectangle(this->_position.x, this->_position.y, dist, 2);
			this->_endcoord1 = Vector2(this->_rect.getRight(), this->_rect.getTop());
			this->_endcoord2 = Vector2(this->_rect.getRight(), this->_rect.getBottom());
		}
		if (this->_direction == direction::LEFT) {
			this->_rect = Rectangle(this->_position.x - dist, this->_position.y, dist, 2);
			this->_endcoord1 = Vector2(this->_rect.getLeft(), this->_rect.getTop());
			this->_endcoord2 = Vector2(this->_rect.getLeft(), this->_rect.getBottom());
		}
		if (this->_direction == direction::UP) {
			this->_rect = Rectangle(this->_position.x, this->_position.y - dist, 2, dist);
			this->_endcoord1 = Vector2(this->_rect.getLeft(), this->_rect.getTop());
			this->_endcoord2 = Vector2(this->_rect.getRight(), this->_rect.getTop());
		}
		if (this->_direction == direction::DOWN) {
			this->_rect = Rectangle(this->_position.x, this->_position.y, 2, dist);
			this->_endcoord1 = Vector2(this->_rect.getLeft(), this->_rect.getBottom());
			this->_endcoord2 = Vector2(this->_rect.getRight(), this->_rect.getBottom());
		}
	}

	bool is_colliding(Rectangle &other) {
		return
			other.is_colliding(this->_endcoord1) ||
			other.is_colliding(this->_endcoord2);
	}


	const Rectangle getRect() const{ return this->_rect; }
	Vector2 getPosition() { return this->_position; }
	Vector2 getEndPos() {
		if (this->_direction == direction::RIGHT) {
			return Vector2(this->_rect.getRight(), this->_rect.getTop());
		}
		else if (this->_direction == direction::LEFT) {
			return Vector2(this->_rect.getLeft(), this->_rect.getTop());
		}
		else if (this->_direction == direction::UP) {
			return Vector2(this->_rect.getLeft(), this->_rect.getTop());
		}
		else if (this->_direction == direction::DOWN) {
			return Vector2(this->_rect.getLeft(), this->_rect.getBottom() - 2);
		}
		else {
			int x = rand() % this->_rect.getWidth() + this->_position.x;
			int y = rand() % this->_rect.getHeight() + this->_position.y;
			return Vector2(x, y);
		}
	}
	const int getDist() const { return this->_dist; }

private:
	Rectangle _rect;
	Vector2 _position;
	Vector2 _endcoord1, _endcoord2;
	direction::Direction _direction;
	int _dist;
};




#endif CORRIDOR_H