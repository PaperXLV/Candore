#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

struct Graphics;

class Rectangle {
public:
	Rectangle() {}
	Rectangle(int x, int y, int width, int height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height)
	{}

	const int getCenterX() const { return this->_x + this->_width / 2; }
	const int getCenterY() const { return this->_y + this->_height / 2; }
	const Vector2 getCenter() const { return Vector2(this->_x + this->_width / 2, this->_y + this->_height / 2); }

	const int getLeft() const { return this->_x; }
	const int getRight() const { return this->_x + this->_width; }
	const int getTop() const { return this->_y; }
	const int getBottom() const { return this->_y + this->_height; }

	const int getHeight() const { return this->_height; }
	const int getWidth() const { return this->_width; }

	const bool isValidRectangle() const {
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	const inline Rectangle getRect() const { return *this; }

	void setXYCoord(float x, float y) {
		this->_x = x; this->_y = y;
	}

	bool is_colliding(Vector2 &point) {
		return point.x > this->getLeft() &&
			point.x < this->getRight() &&
			point.y > this->getTop() &&
			point.y < this->getBottom();
	}

private:
	float _x, _y, _width, _height;
};




#endif RECTANGLE_H