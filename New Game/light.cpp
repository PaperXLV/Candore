#pragma once
#include "light.h"
#include <iostream>

Light::Light() {}

Light::Light(Graphics &graphics, Vector2 spawnpos) :
	AnimatedSprite(graphics, "Assets/sprites/stoic/Hanging_Light.png", 0, 0, 32, 32, spawnpos, 100),
	_center(Vector2(this->_position.x + 16, this->_position.y + 16)),
	_on(true),
	_lightingChange(true)
{
	this->setupAnimations();
	this->playAnimation("Idle");
	//add raycasts and whatnot we need
}

Light::~Light() {}

void Light::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 32, 32, Vector2(0, 0));
}

void Light::animationDone(std::string currentAnimation) {}

void Light::addCollider(Collider collider) {
	this->_nearby.push_back(collider);
}

void Light::initializeRays() {
	for (int i = 0; i < this->_nearby.size(); i++) {
		std::vector<Vector2> corners = this->_nearby[i].getCorners();

		for (int j = 0; j < corners.size(); j++) {
			Raycast newRay = Raycast(this->_center, corners[j]);
			this->_lightRays.push_back(Raycast(this->_center, newRay.getRadians() - 0.000001f, level_constants::DIST_OF_LIGHT));
			this->_lightRays.push_back(newRay);
			this->_lightRays.push_back(Raycast(this->_center, newRay.getRadians() + 0.000001f, level_constants::DIST_OF_LIGHT));
		}
	}
	for (int i = 0; i < this->_lightRays.size(); i++) {
		this->_lightRays[i].stopOnCollision(this->_nearby);
	}

	std::sort(this->_lightRays.begin(), this->_lightRays.end(), [](const Raycast &a, const Raycast &b) {
		return a.getRadians() < b.getRadians();
	});

	this->_lightPolygonx.clear();
	this->_lightPolygony.clear();
	for (int i = 0; i < this->_lightRays.size(); i++) {
		this->_lightPolygonx.push_back(this->_lightRays[i].getEarlyStop().x);
		this->_lightPolygony.push_back(this->_lightRays[i].getEarlyStop().y);
	}
}

Polygon Light::generateLightPolygon(Vector2 offset) {
	//black magic hackery that I can just make a pointer to the first member and we good
	std::vector<Sint16> tempX;
	std::vector<Sint16> tempY;
	for (int i = 0; i < this->_lightPolygonx.size(); i++) {
		tempX.push_back(this->_lightPolygonx[i]);
		tempY.push_back(this->_lightPolygony[i]);
	}

	try {
		Sint16 *vx = &tempX.at(0);
		Sint16 *vy = &tempY.at(0);
		return Polygon(vx, vy, tempX.size());
	}
	catch (std::out_of_range& oor) {
		printf("out of range");
		return Polygon();
	}	
}

void Light::update(int elapsedTime) {
	//update all the stuff for lighting
	//call stopOnCollision for a vector of any colliders that change, as _nearby and _position don't so no reason to call every cycle

	if (this->_lightingChange) {
		this->_lightPolygonx.clear();
		this->_lightPolygony.clear();
		for (int i = 0; i < this->_lightRays.size(); i++) {
			this->_lightPolygonx.push_back(this->_lightRays[i].getEarlyStop().x);
			this->_lightPolygony.push_back(this->_lightRays[i].getEarlyStop().y);
		}
	}
	this->_lightingChange = false;
	AnimatedSprite::update(elapsedTime);
}

void Light::draw(Graphics &graphics, Vector2 offset) {
	//TODO:: configure the draw and alpha channels and such to make lighting work with what we got
	AnimatedSprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
}