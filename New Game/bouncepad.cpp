#include "bouncepad.h"

BouncePad::BouncePad() {}

BouncePad::BouncePad(Graphics &graphics, Vector2 spawnpos) :
	AnimatedSprite(graphics, "Assets/sprites/stoic/Bounce_Pad.png", 0, 0, 32, 32, spawnpos, 100),
	_bounceParticle(graphics, Vector2(spawnpos.x, spawnpos.y - 32)),
	_blockCollider(spawnpos, Rectangle(spawnpos.x, spawnpos.y, this->_width, this->_height)),
	_bounceCollider(spawnpos, Rectangle(spawnpos.x + 1, spawnpos.y - 1, this->_width - 2, 4))
{
	this->setupAnimations();
	this->playAnimation("Idle");
}

BouncePad::~BouncePad() {}

void BouncePad::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 32, 32, Vector2(0, 0));
}

void BouncePad::animationDone(std::string currentAnimation) {}

void BouncePad::update(int elapsedTime) {
	this->_bounceParticle.update(elapsedTime);
	AnimatedSprite::update(elapsedTime);
}

void BouncePad::draw(Graphics &graphics, Vector2 offset) {
	this->_bounceParticle.draw(graphics, offset);
	AnimatedSprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BounceParticle::BounceParticle() {}

BounceParticle::BounceParticle(Graphics &graphics, Vector2 spawnpos) :
	AnimatedSprite(graphics, "Assets/sprites/animated/Pad_Particles.png", 0, 0, 32, 32, spawnpos, 100)
{
	this->setupAnimations();
	this->playAnimation("Particle");
}

BounceParticle::~BounceParticle() {}

void BounceParticle::setupAnimations() {
	this->addAnimation(16, 0, 0, "Particle", 32, 32, Vector2(0, 0));
}

void BounceParticle::animationDone(std::string currentAnimation) {}

void BounceParticle::update(int elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void BounceParticle::draw(Graphics &graphics, Vector2 offset) {
	AnimatedSprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
}