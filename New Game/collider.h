#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include "rectangle.h"
#include <map>

class Collider {
public:
	Collider();
	Collider(Vector2 ownerPos, Rectangle pos);

	void generateCorners(std::map<std::vector<int>, int> levelMap);

	void update(Vector2 ownerPos);

	void resetCollisions();

	const Rectangle getPosition() const { return this->_position; }
	const Vector2 getChildDist() const { return this->_childDist; }
	const std::vector<Vector2> getCorners() const { return this->_corners; }

	bool is_colliding(Collider &other) const;

	bool collidingLeft;
	bool collidingRight;
	bool collidingTop;
	bool collidingBottom;

private:
	std::vector<Vector2> _corners;
	Rectangle _position;
	Vector2 _childDist;
};

#endif COLLIDER_H