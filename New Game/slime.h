#pragma once
#ifndef SLIME_H
#define SLIME_H

#include "enemy.h"

namespace Slime_Animations {
	enum SAnimation {
		Idle,
		Hit,
		NONE
	};
}

class Slime : public Enemy {
public:
	Slime();
	Slime(Graphics &graphics, int sourceX, int sourceY, int width, int height,
		Vector2 spawnpos, int health);
	~Slime();

	void setupAnimations();
	void animationDone(std::string currentAnimation);

	void decideAction();

	void jump(faces::Face jumpFacing);
	void hit(int damage);

	void update(int elapsedTime, Level &level);
	void draw(Graphics &graphics, Vector2 offset);
private:
	std::map<Slime_Animations::SAnimation, std::string> _animationMap;
};


#endif SLIME_H