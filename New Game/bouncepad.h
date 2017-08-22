#pragma once
#include "animatedsprite.h"
#include "collider.h"

class BounceParticle : public AnimatedSprite {
public:
	BounceParticle();
	BounceParticle(Graphics &graphics, Vector2 spawnpos);
	~BounceParticle();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void update(int elapsedTime);
	void draw(Graphics &graphics, Vector2 offset);
};

class BouncePad : public AnimatedSprite {
public:
	BouncePad();
	BouncePad(Graphics &graphics, Vector2 spawnpos);
	~BouncePad();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	const Collider getCollider() const { return this->_blockCollider; }
	const Collider getBounceCollider() const { return this->_bounceCollider; }

	void update(int elapsedTime);
	void draw(Graphics &graphics, Vector2 offset);

private:
	BounceParticle _bounceParticle;
	Collider _blockCollider;
	Collider _bounceCollider;

};