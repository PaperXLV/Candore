#pragma once
#ifndef ROTATIONRECT_H
#define ROTATIONRECT_H

#include "globals.h"
#include "raycast.h"
#include "sprite.h"
#include <iostream>

struct Graphics;

class RotationRect {
public:
	RotationRect() {}
	RotationRect(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, int length, Graphics &graphics) :
		_p1(p1),
		_p2(p2),
		_p3(p3),
		_p4(p4),
		_length(length),
		_p1s(graphics, "Assets/sprites/animated/player/4by4marker.png", 0, 0, 4, 4, this->_p1),
		_p2s(graphics, "Assets/sprites/animated/player/4by4marker.png", 0, 0, 4, 4, this->_p2),
		_p3s(graphics, "Assets/sprites/animated/player/4by4marker.png", 0, 0, 4, 4, this->_p3),
		_p4s(graphics, "Assets/sprites/animated/player/4by4marker.png", 0, 0, 4, 4, this->_p4)
	{
		this->_rectSides = { Raycast(this->_p1, this->_p2), Raycast(this->_p2, this->_p3), Raycast(this->_p3, this->_p4), Raycast(this->_p4, this->_p1) };

	}
	~RotationRect() {}

	void update(Vector2 center, float rotation) {
		//find some way to calculate where the points are based on the player's position and rotation
		//Previous
		/*
		this->_p1 = position;
		this->_p2 = Vector2(this->_p1.x + cos(globals::degtorad(rotation)) * this->_length, this->_p1.y + sin(globals::degtorad(rotation)) * this->_length);
		this->_p3 = Vector2(this->_p2.x - sin(globals::degtorad(rotation)) * this->_length, this->_p2.y + cos(globals::degtorad(rotation)) * this->_length);
		this->_p4 = Vector2(this->_p1.x - cos(globals::degtorad(90 - rotation)) * this->_length, this->_p1.y + sin(globals::degtorad(90 - rotation)) * this->_length);
		//these might be slightly off with the signs but idk
		*/
		float radius = sqrt(128);


		this->_p1 = Vector2(center.x + cos(globals::degtorad(rotation + 45)) * radius, center.y + sin(globals::degtorad(rotation + 45)) * radius);
		this->_p2 = Vector2(center.x + cos(globals::degtorad(rotation + 135)) * radius, center.y + sin(globals::degtorad(rotation + 135)) * radius);
		this->_p3 = Vector2(center.x + cos(globals::degtorad(rotation + 225)) * radius, center.y + sin(globals::degtorad(rotation + 225)) * radius);
		this->_p4 = Vector2(center.x + cos(globals::degtorad(rotation + 315)) * radius, center.y + sin(globals::degtorad(rotation + 315)) * radius);


		this->_rectSides.clear();
		this->_rectSides = { Raycast(this->_p1, this->_p2), Raycast(this->_p2, this->_p3), Raycast(this->_p3, this->_p4), Raycast(this->_p4, this->_p1) };

		this->_center = Vector2((this->_p1.x + this->_p2.x + this->_p3.x + this->_p4.x) / 4, (this->_p1.y + this->_p2.y + this->_p3.y + this->_p4.y) / 4);

		//Debug shit
		/*
		this->_p1s.setDistRectX(this->_p1.x); this->_p1s.setDistRectY(this->_p1.y);
		this->_p2s.setDistRectX(this->_p2.x); this->_p2s.setDistRectY(this->_p2.y);
		this->_p3s.setDistRectX(this->_p3.x); this->_p3s.setDistRectY(this->_p3.y);
		this->_p4s.setDistRectX(this->_p4.x); this->_p4s.setDistRectY(this->_p4.y);

		this->_p1s.update(); this->_p2s.update(); this->_p3s.update(); this->_p4s.update();
		*/


	}
	void getDebuggingPointsToDraw(std::vector<Vector2> points) {
		//debug
		this->_segmentedSidesPoints.clear();
		this->_segmentedSidesPoints = points;
	}


	const std::vector<Raycast> getRectSides() const { return this->_rectSides; }
	const std::vector<Vector2> getPoints() const { return std::vector<Vector2> { this->_p1, this->_p2, this->_p3, this->_p4 }; }
	const std::vector<float> getYPoints() const { return std::vector<float> { this->_p1.y, this->_p2.y, this->_p3.y, this->_p4.y}; }
	const std::vector<float> getXPoints() const { return std::vector<float> { this->_p1.x, this->_p2.x, this->_p3.x, this->_p4.x}; }
	const Vector2 getCenter() const { return this->_center; }
	
	
	const Vector2 getLowestPoint() const {
		Vector2 lowest = this->_p1;
		for (int i = 0; i < this->getPoints().size(); i++) {
			if (this->getPoints()[i].y > lowest.y) {
				lowest = this->getPoints()[i];
			}
		}
		return lowest;
	}

	const Vector2 getMiddleOfLowestSide() const {
		Vector2 lowestp = this->_p1;
		int indexLowest = 0;
		for (int i = 0; i < this->getPoints().size(); i++) {
			if (this->getPoints()[i].y > lowestp.y) {
				lowestp = this->getPoints()[i];
				indexLowest = i;
			}
		}
		if (this->getPoints()[(indexLowest + 1) % 4].y > this->getPoints()[(indexLowest + 3) % 4].y) {
			return Vector2((this->getPoints()[indexLowest].x + getPoints()[(indexLowest + 1) % 4].x) / 2, (this->getPoints()[indexLowest].y + getPoints()[(indexLowest + 1) % 4].y) / 2);
		}
		else {
			return Vector2((this->getPoints()[indexLowest].x + getPoints()[(indexLowest + 3) % 4].x) / 2, (this->getPoints()[indexLowest].y + getPoints()[(indexLowest + 3) % 4].y) / 2);
		}
	}

	const std::vector<Raycast> getRaysByPoint(int point) {
		//You're gonna need to derive this at some point
		if (point == 1) {
			return std::vector<Raycast> { Raycast(this->_p1, Vector2((this->_p1.x + this->_p2.x) / 2, (this->_p1.y + this->_p2.y) / 2)), Raycast(this->_p1, Vector2((this->_p1.x + this->_p4.x) / 2, (this->_p1.y + this->_p4.y) / 2))};
		}
		else if (point == 2) {
			return std::vector<Raycast> { Raycast(this->_p2, Vector2((this->_p2.x + this->_p1.x) / 2, (this->_p2.y + this->_p1.y) / 2)), Raycast(this->_p2, Vector2((this->_p2.x + this->_p3.x) / 2, (this->_p2.y + this->_p3.y) / 2))};
		}
		else if (point == 3) {
			return std::vector<Raycast> { Raycast(this->_p3, Vector2((this->_p3.x + this->_p2.x) / 2, (this->_p3.y + this->_p2.y) / 2)), Raycast(this->_p3, Vector2((this->_p3.x + this->_p4.x) / 2, (this->_p3.y + this->_p4.y) / 2))};
		}
		else if (point == 4) {
			return std::vector<Raycast> { Raycast(this->_p4, Vector2((this->_p4.x + this->_p3.x) / 2, (this->_p4.y + this->_p3.y) / 2)), Raycast(this->_p4, Vector2((this->_p4.x + this->_p1.x) / 2, (this->_p4.y + this->_p1.y) / 2))};
		}
	}

	void draw(Graphics &graphics, Vector2 offset) {
		/*
		this->_p1s.draw(graphics, this->_p1s.getX() - offset.x, this->_p1s.getY() - offset.y);
		
		this->_p2s.draw(graphics, this->_p2s.getX() - offset.x, this->_p2s.getY() - offset.y);
		this->_p3s.draw(graphics, this->_p3s.getX() - offset.x, this->_p3s.getY() - offset.y);
		this->_p4s.draw(graphics, this->_p4s.getX() - offset.x, this->_p4s.getY() - offset.y);
		*/
		//Debug
		/*
		this->_segmentedSides.clear();
		for (int i = 0; i < this->_segmentedSidesPoints.size(); i++) {
			this->_segmentedSides.push_back(Sprite(graphics, "Assets/sprites/animated/player/4by4marker.png", 0, 0, 4, 4, this->_segmentedSidesPoints[i]));
			this->_segmentedSides[i].draw(graphics, this->_segmentedSides[i].getX() - offset.x, this->_segmentedSides[i].getY() - offset.y);
		}
		*/
	}

private:
	Vector2 _p1, _p2, _p3, _p4;
	Vector2 _center;
	std::vector<Raycast> _rectSides;

	std::vector<Sprite> _segmentedSides;
	std::vector<Vector2> _segmentedSidesPoints;

	int _length;
	Sprite _p1s, _p2s, _p3s, _p4s;
};


#endif ROTATIONRECT_H