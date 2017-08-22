#include "legs.h"

Legs::Legs() {}

Legs::Legs(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnpos, Vector2 difference) :
	AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, Vector2(spawnpos.x + difference.x, spawnpos.y + difference.y), 100),
	_difference(difference),
	_playerFacing(faces::LEFT)
{
	this->setupAnimations();
	this->idle();
}

Legs::~Legs() {}

void Legs::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle_Left", 40, 40, Vector2(0, 0));
	this->addAnimation(1, 1, 0, "Idle_Right", 40, 40, Vector2(0, 0));
	this->addAnimation(1, 2, 0, "Fall_Left", 40, 40, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "Jump_Left", 40, 40, Vector2(0, 0));
	this->addAnimation(1, 4, 0, "Fall_Right", 40, 40, Vector2(0, 0));
	this->addAnimation(1, 5, 0, "Jump_Right", 40, 40, Vector2(0, 0));
	this->addAnimation(8, 0, 40, "Run_Left", 40, 40, Vector2(0, 0));
	this->addAnimation(8, 0, 80, "Run_Right", 40, 40, Vector2(0, 0));
}

void Legs::animationDone(std::string currentAnimation) {

}

void Legs::idle(faces::Face changeFace) {
	if (changeFace != faces::NONE) {
		this->_playerFacing = changeFace;
	}
	if (this->_playerFacing == faces::LEFT) {
		if (this->_currentAnimation != "Idle_Left") {
			this->playAnimation("Idle_Left");
		}
	}
	else if (this->_playerFacing == faces::RIGHT) {
		if (this->_currentAnimation != "Idle_Right") {
			this->playAnimation("Idle_Right");
		}
	}
}

void Legs::jump(faces::Face changeFace) {
	if (changeFace != faces::NONE) {
		this->_playerFacing = changeFace;
	}
	if (this->_playerFacing == faces::LEFT) {
		if (this->_currentAnimation != "Jump_Left") {
			this->playAnimation("Jump_Left");
		}
	}
	else if (this->_playerFacing == faces::RIGHT) {
		if (this->_currentAnimation != "Jump_Right") {
			this->playAnimation("Jump_Right");
		}
	}
}

void Legs::fall(faces::Face changeFace) {
	if (changeFace != faces::NONE) {
		this->_playerFacing = changeFace;
	}
	if (this->_playerFacing == faces::LEFT) {
		if (this->_currentAnimation != "Fall_Left") {
			this->playAnimation("Fall_Left");
		}
	}
	else if (this->_playerFacing == faces::RIGHT) {
		if (this->_currentAnimation != "Fall_Right") {
			this->playAnimation("Fall_Right");
		}
	}
}

void Legs::run(faces::Face changeFace) {
	if (changeFace != faces::NONE) {
		this->_playerFacing = changeFace;
	}
	if (this->_playerFacing == faces::LEFT) {
		if (this->_currentAnimation != "Run_Left") {
			this->playAnimation("Run_Left");
		}
	}
	else if (this->_playerFacing == faces::RIGHT) {
		if (this->_currentAnimation != "Run_Right") {
			this->playAnimation("Run_Right");
		}
	}
}

void Legs::update(Vector2 position, int elapsedTime, faces::Face playerFacing) {
	this->_position = Vector2(position.x + this->_difference.x, position.y + this->_difference.y);
	this->_playerFacing = playerFacing;
	AnimatedSprite::update(elapsedTime);
}

void Legs::draw(Graphics &graphics, Vector2 offset) {
	AnimatedSprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
}