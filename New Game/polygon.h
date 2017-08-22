#pragma once
#ifndef POLYGON_H
#define POLYGON_H
#include <SDL.h>

struct Polygon {
	const Sint16 *x, *y;
	int points;

	Polygon(const Sint16 *vx, const Sint16 *vy, int numberOfPoints) :
		x(vx),
		y(vy),
		points(numberOfPoints)
	{}

	Polygon() :
		x(0),
		y(0),
		points(0)
	{}

};


#endif POLYGON_H