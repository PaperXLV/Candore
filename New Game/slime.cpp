#include "slime.h"

#include <iostream>

Slime::Slime() {}

Slime::Slime(Graphics &graphics, int sourceX, int sourceY, int width, int height,
	Vector2 spawnpos, int health) :
	Enemy(graphics, "Assets/sprites/animated/Enemies/Green_Square.png", sourceX, sourceY, width, height, spawnpos, health)
{
	this->setupAnimations();

	this->_animationMap[Slime_Animations::Idle] = "Idle";
	this->_animationMap[Slime_Animations::Hit] = "Hit";


	this->playAnimation(this->_animationMap[Slime_Animations::Idle]);
}

Slime::~Slime() {}

void Slime::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 40, 40, Vector2(0, 0));
	this->addAnimation(6, 1, 0, "Hit", 40, 40, Vector2(0, 0));
}

void Slime::animationDone(std::string currentAnimation) {
	if (currentAnimation == this->_animationMap[Slime_Animations::Hit]) {
		this->playAnimation(this->_animationMap[Slime_Animations::Idle]);
	}
}

void Slime::decideAction() {
	//woo AI leggo
}

void Slime::jump(faces::Face jumpFacing) {
	//add in jump
}

void Slime::hit(int damage) {
	if (this->_currentAnimation != this->_animationMap[Slime_Animations::Hit]) {
		this->playAnimation(this->_animationMap[Slime_Animations::Hit], true);
		this->damaged(damage);
	}
}

void Slime::update(int elapsedTime, Level &level) {
	/* impliment
	if (this->_currentAnimation != this->_animationMap[Slime_Animations::Hit]) {
		for (int i = 0; i < playerSlashes.size(); i++) {
			if (this->getCollider().is_colliding(playerSlashes[i])) {
				//don't hard code damage, read from player's weapon

				//Solution to continuous damage taking
				//When takes damage, play damaged animation
				//Unable to take damage during animation!
				this->hit(1);
				//break out of loop
				i = playerSlashes.size();
			}
		}
	}

	*/
	Enemy::update(elapsedTime, level);
}

void Slime::draw(Graphics &graphics, Vector2 offset) {
	Enemy::draw(graphics, offset);
}