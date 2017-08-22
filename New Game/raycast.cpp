#include "raycast.h"
#include <math.h>
#include <iostream>

//Raycast to check for collision for player/moving objects
Raycast::Raycast(Vector2 start, Vector2 dir, float len) :
	_start(start),
	_dir(dir),
	_len(len)
{

}

//Raycast to use for lighting
Raycast::Raycast(Vector2 start, Vector2 stop) :
	_start(start),
	_stop(stop),
	_earlyStop(stop)
{
	float absxdiff = (stop.x - start.x);
	float absydiff = (stop.y - start.y);

	this->_rad = atan2(absydiff, absxdiff);
	
}

//Lighting cont.
Raycast::Raycast(Vector2 start, float radians, float length) :
	_start(start),
	_rad(radians)
{
	this->_stop = Vector2(this->_start.x + (length * cos(radians)), this->_start.y + (length * sin(radians)));
	this->_earlyStop = this->_stop;
}

//Lighting update
void Raycast::update(Vector2 start, Vector2 stop) {
	this->_start = start;
	this->_stop = stop;

	float xdiff = stop.x - start.x;
	float ydiff = stop.y - start.y;
	this->_rad = atan2(ydiff, xdiff);
}

//Collision checker raycast update
void Raycast::update(Vector2 centerObject, faces::Face face) {
	this->_start = centerObject;
	if (face == faces::LEFT) {
		this->_dir = Vector2(-1.0f, 0);
		this->_rad = PI;
	}
	else if (face == faces::RIGHT) {
		this->_dir = Vector2(1.0f, 0);
		this->_rad = 0.0f;
	}
}

//Check if start or stop point is inside rect
const bool Raycast::collidesWith(const Rectangle &other) const {
	return
		(this->_start.x <= other.getRight() &&
			this->_start.x >= other.getLeft() &&
			this->_start.y <= other.getBottom() &&
			this->_start.y >= other.getTop()) ||
			(this->_stop.x <= other.getRight() &&
				this->_stop.x >= other.getLeft() &&
				this->_stop.y <= other.getBottom() &&
				this->_stop.y >= other.getTop());
}
//Check collision
bool Raycast::is_colliding(Collider collider) {
	Vector2 endpoint = Vector2(this->_start.x + (this->_dir.x * this->_len), this->_start.y + (this->_dir.y * this->_len));
	this->_stop = endpoint;
	if (this->collidesWith(collider.getPosition())) {
		this->_distanceY = this->getVertCollidingDistance(collider);
		this->_distanceX = this->getHorizCollidingDistance(collider);
		return true;
	}
	return false;
}

//Split Raycast into divisor parts
std::vector<Vector2> Raycast::segment(int divisor) {
	std::vector<Vector2> segmented;
	for (int i = 0; i < divisor; i++) {
		segmented.push_back(Vector2(this->_start.x + ((this->_stop.x - this->_start.x) / divisor * i), this->_start.y + ((this->_stop.y - this->_start.y) / divisor * i)));
	}
	return segmented;
}


float Raycast::getDistance(const Rectangle &other) {
	return 1;
}

void Raycast::stopOnCollision(std::vector<Collider> nearby) {
	float lowestT = 9000;

	for (int i = 0; i < nearby.size(); i++) {
		Rectangle current = nearby[i].getPosition();

		Raycast left = Raycast(Vector2(current.getLeft(), current.getTop()), Vector2(current.getLeft(), current.getBottom()));
		Raycast right = Raycast(Vector2(current.getRight(), current.getTop()), Vector2(current.getRight(), current.getBottom()));
		Raycast top = Raycast(Vector2(current.getLeft(), current.getTop()), Vector2(current.getRight(), current.getTop()));
		Raycast bottom = Raycast(Vector2(current.getLeft(), current.getBottom()), Vector2(current.getRight(), current.getBottom()));

		std::vector<Raycast> parametricSegments{ left, right, top, bottom };

		for (int j = 0; j < parametricSegments.size(); j++) {
			Raycast segment = parametricSegments[j];
			//NEED TO KNOW THE DIRECTION OF THE RAY WE'RE SENDING TO THE POINT, INIT WITH Raycast(Vector2 start, Vector2 stop)
			Vector2 dRay = Vector2(this->_stop.x - this->_start.x, this->_stop.y - this->_start.y);
			Vector2 dSegment = Vector2(segment.getStop().x - segment.getStart().x, segment.getStop().y - segment.getStart().y);

			//check if not parallel
			if (!(dRay.x == dSegment.x && dRay.y == dSegment.y)) {
				//lots of math here
				float T2 = (dRay.x * (segment.getStart().y - this->_start.y) + dRay.y * (this->_start.x - segment.getStart().x)) / ((dSegment.x * dRay.y) - (dSegment.y * dRay.x));

				float T1 = (segment.getStart().x + (dSegment.x * T2) - this->_start.x) / dRay.x;

				if (T2 <= 1 && T2 >= 0) { //between the start and stop of original ray
					if (T1 < lowestT && T1 >= 0) {
						lowestT = T1;
					}
				}
			}
		}
	}
	if (lowestT != 9000) {
		Vector2 dRay = Vector2(this->_stop.x - this->_start.x, this->_stop.y - this->_start.y);
		this->_earlyStop = Vector2(this->_start.x + (dRay.x * lowestT), this->_start.y + (dRay.y * lowestT));
	}
}

float Raycast::getVertCollidingDistance(const Collider &other) {
	Rectangle currentRect = other.getPosition();
	float distance;
	if (this->_dir.y > 0) {
		distance = currentRect.getTop() - this->_start.y;
	}
	else if (this->_dir.y < 0) {
		distance = currentRect.getBottom() - this->_start.y;
	}
	else {
		distance = -1;
	}

	return distance;
}

float Raycast::getHorizCollidingDistance(const Collider &other) {
	Rectangle currentRect = other.getPosition();
	float distance;
	if (this->_dir.x > 0) {
		distance = currentRect.getLeft() - this->_start.x;
	}
	else if (this->_dir.x < 0) {
		distance = currentRect.getRight() - this->_start.x;
	}
	else {
		distance = -1;
	}
	
	return distance;
}