#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "raycast.h"
#include "rotationrect.h"
#include "collider.h"
#include "animatedsprite.h"
#include "globals.h"
#include "input.h"
#include "level.h"
#include "slash.h"
#include "legs.h"

class Weapon : public AnimatedSprite {
public:
	Weapon();
	Weapon(Graphics &graphics, Vector2 spawn);
	~Weapon();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void draw(Graphics &graphics, Vector2 point, Vector2 offset, SDL_Point center, float rotation);
};

class Player : public AnimatedSprite{
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnpoint);
	~Player() {}

	void draw(Graphics &graphics, Vector2 offset);
	void update(int elapsedTime, Level &level, Input &input);

	//different move stuff

	const float getX() const;
	const float getY() const;

	const float getCenterX() const;
	const float getCenterY() const;
	const Vector2 getCenter() const;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	const Collider getCollider() const { return this->_collider; }

	void preformMovement(Input &input, int elapsedTime);
	void aerialMovement(Input &input, int elapsedTime);
	void moveLeft(int elapsedTime, bool holding);
	void moveRight(int elapsedTime, bool holding);
	void jump();
	void bounce();
	void stop(int elapsedTime);
	void startFalling();
	void stayGrounded(int elapsedTime);

	bool is_grounded(int elapsedTime);

	const Vector2 getPosition() const { return this->_position; }

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	Vector2 calculateCollision(int elapsedTime);

	bool alive();

private:
	Vector2 _rightHandPos;
	Vector2 _leftHandPos;
	Vector2 _velocity;
	Collider _collider;
	faces::Face _currentFacing;

	RotationRect _rotationRect;

	Weapon _weapon;
	
	// probably not gonna need these once transition to rect is complete

	std::vector<Collider> _nearby;
	std::vector<Collider> _nearbyBounce;

	bool _bounce;
	bool _jumping;
	bool _movingLeft;
	bool _movingRight;

	bool _disableGrounded;

	int _maxHealth, _currentHealth;

	float _currentRotationDeg;
	float _ms_to_deg;
	float _ms_to_dist;
};







#endif PLAYER_H