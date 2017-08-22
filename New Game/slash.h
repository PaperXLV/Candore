#pragma once
#ifndef SLASH_H
#define SLASH_H

#include "animatedsprite.h"

class Slash : public AnimatedSprite {
public:
	Slash() {}
	Slash(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
		Vector2 spawnpos);
	~Slash();
	

	void setupAnimations();
	void animationDone(const std::string currentAnimation);
	
	bool attacking();
	bool pausing();

	void attack(weapons::WeaponName currentWeapon);
	//Animations
	void sword1();
	void sword2();

	void update(Vector2 position, faces::Face currentFacing, int elapsedTime);
	void draw(Graphics &graphics, Vector2 offset);

	const std::string getCurrentAnimation() const { return this->_currentAnimation; }

private:
	faces::Face _playerFacing;
	std::string _lastAttack;
};





#endif SLASH_H