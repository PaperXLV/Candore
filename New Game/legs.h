#pragma once
#ifndef LEGS_H
#define LEGS_H

#include "animatedsprite.h"

class Legs : public AnimatedSprite {
public:
	Legs();
	Legs(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
		Vector2 spawnpos, Vector2 difference);
	~Legs();

	void setupAnimations();
	void animationDone(std::string currentAnimation);

	void run(faces::Face changeFace = faces::NONE);
	void jump(faces::Face changeFace = faces::NONE);
	void idle(faces::Face changeFace = faces::NONE);
	void fall(faces::Face changeFace = faces::NONE);

	void update(Vector2 position, int elapsedTime, faces::Face playerFacing);
	void draw(Graphics &graphics, Vector2 offset);

	const std::string getCurrentAnimation() const { return this->_currentAnimation; }

private:
	faces::Face _playerFacing;
	Vector2 _difference;
};




#endif LEGS_H