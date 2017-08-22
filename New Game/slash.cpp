#include "slash.h"
#include <iostream>

Slash::Slash(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnpos) :
	AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, Vector2(spawnpos.x - 10, spawnpos.y + 20), 50), //editing spawnpos position to line up where the slashes should be
	_lastAttack("")
{
	this->setupAnimations();
	this->playAnimation("Idle");
}

Slash::~Slash() {}

void Slash::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 32, 32, Vector2(0, 0));
	this->addAnimation(3, 1, 0, "Sword1", 32, 32, Vector2(0, 0));
	this->addAnimation(3, 0, 32, "Sword2", 32, 32, Vector2(0, 0));
	this->addAnimation(6, 0, 0, "Pause", 1, 32, Vector2(0, 0));
}
                                                                                    
void Slash::animationDone(std::string currentAnimation) {
	if (currentAnimation == "Sword1" || currentAnimation == "Sword2") {
		this->playAnimation("Pause");
	}
	if (currentAnimation == "Pause") {
		this->playAnimation("Idle");
	}
}

bool Slash::attacking() {
	return (this->_currentAnimation == "Sword1" || this->_currentAnimation == "Sword2");
	//add additional animations when created
}
bool Slash::pausing() {
	return (this->_currentAnimation == "Pause");
}


void Slash::attack(weapons::WeaponName currentWeapon) {
	if (currentWeapon == weapons::Sword) {
		if (this->_currentAnimation == "Pause") {
			if (this->_lastAttack == "Sword1") {
				this->sword2();
			}
			else if (this->_lastAttack == "Sword2") {
				this->sword1();
			}
			else {
				this->sword1();
			}
		}
		else {
			this->sword1();
		}
	}
}

void Slash::sword1() {
	this->playAnimation("Sword1", true);
	this->_lastAttack = "Sword1";
}

void Slash::sword2() {
	this->playAnimation("Sword2", true);
	this->_lastAttack = "Sword2";
}

void Slash::update(Vector2 position, faces::Face currentFacing, int elapsedTime) {
	this->_position = position;
	this->_playerFacing = currentFacing;
	if (this->_playerFacing == sides::RIGHT) {
		this->_position.x += 34;
		//moving slash over to the other side of player
	}
	if (this->_playerFacing == sides::LEFT) {
		this->_position.x -= 30;
	}
	//edit based on facing
	
	AnimatedSprite::update(elapsedTime);
}

void Slash::draw(Graphics &graphics, Vector2 offset) {
	SDL_Point center = { 0, 0 };
	SDL_RendererFlip flip;
	(this->_playerFacing == sides::LEFT) ? flip = SDL_FLIP_NONE : flip = SDL_FLIP_HORIZONTAL;

	AnimatedSprite::drawRotate(graphics, this->_position.x - offset.x, this->_position.y - offset.y, 0, center, flip);
}