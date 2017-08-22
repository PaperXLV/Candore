#include "door.h"

Door::Door() {}

Door::Door(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnpos) :
	AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnpos, 100)
{
	this->setupAnimations();
	this->playAnimation("Idle");
}

Door::~Door() {}

void Door::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 32, 64, Vector2(0, 0));
}

void Door::animationDone(std::string currentanimation) {

}

void Door::update(int elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void Door::draw(Graphics &graphics, Vector2 offset) {
	AnimatedSprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
}