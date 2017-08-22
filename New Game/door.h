#pragma once
#ifndef DOOR_H
#define DOOR_H

#include "animatedsprite.h"

class Door : public AnimatedSprite {
public:
	Door();
	Door(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnpos);
	~Door();

	void animationDone(std::string currentanimation);
	void update(int elapsedTime);
	void draw(Graphics &graphics, Vector2 offset);

	void setupAnimations();


private:
	
};



#endif DOOR_H