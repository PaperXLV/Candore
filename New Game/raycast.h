#pragma once
#ifndef RAYCAST_H
#define RAYCAST_H

#include "globals.h"
#include "collider.h"
#include <vector>

namespace ray_constants {
	const float distBetweenRays = 1.0f;
}


class Raycast {
public:
	Raycast() {}
	Raycast(Vector2 start, Vector2 stop);
	Raycast(Vector2 start, Vector2 dir, float length);
	Raycast(Vector2 start, float radians, float length);

	void update(Vector2 start, Vector2 stop);
	void update(Vector2 centerObject, faces::Face face);

	float getVertCollidingDistance(const Collider &other);
	float getHorizCollidingDistance(const Collider &other);
	float getDistance(const Rectangle &other);

	bool is_colliding(Collider collider);
	const bool collidesWith(const Rectangle &other) const;

	std::vector<Vector2> segment(int divisor);

	const inline float getDistanceY() const { return this->_distanceY; }
	const inline float getDistanceX() const { return this->_distanceX; }
	const inline Vector2 getStart() const { return this->_start; }
	const inline Vector2 getStop() const { return this->_stop; }
	const inline Vector2 getEarlyStop() const { return this->_earlyStop; }
	const inline float getRadians() const { return this->_rad; }


	void stopOnCollision(std::vector<Collider> nearby);

private:
	Vector2 _start;
	Vector2 _dir;
	Vector2 _stop;
	Vector2 _earlyStop;
	float _len;
	float _rad;
	bool _is_enabled;
	float _distanceY, _distanceX;
};


#endif RAYCAST_H