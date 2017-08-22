#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "animatedsprite.h"
#include "raycast.h"
#include <algorithm>

class Light : public AnimatedSprite {
public:
	Light();
	Light(Graphics &graphics, Vector2 spawnpos);
	~Light();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	const Vector2 getCenter() const { return this->_center; }

	void addCollider(Collider collider);
	void initializeRays();

	bool sortbyrad(const Raycast &a, const Raycast &b) {
		return a.getRadians() > b.getRadians();
	}

	Polygon generateLightPolygon(Vector2 offset);

	void update(int elapsedTime);
	void draw(Graphics &graphics, Vector2 offset);

private:
	Vector2 _center;

	bool _on;
	bool _lightingChange;

	std::vector<Collider> _nearby;
	std::vector<Raycast> _lightRays;
	std::vector<Sint16> _lightPolygonx;
	std::vector<Sint16> _lightPolygony;

};



#endif LIGHT_H