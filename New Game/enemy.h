#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "collider.h"
#include "level.h"
#include "raycast.h"

class Enemy : public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
		Vector2 spawnpos, int health);
	~Enemy();

	void draw(Graphics &graphics, Vector2 offset);
	void update(int elapsedTime, Level &level);

	//different move stuff

	const float getX() const;
	const float getY() const;

	const float getCenterX() const;
	const float getCenterY() const;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	void damaged(int damage);

	const Collider getCollider() const { return this->_collider; }

	const Vector2 getPosition() const { return this->_position; }

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	float verticalCollisions();
	float horizontalCollisions();

	bool checkSightOfPlayer(Collider &playerCollider);
	bool alive();

private:
	int _currentHealth, _maxHealth;
	Collider _collider;
	std::vector<Collider> _nearby;

	Vector2 _velocity;
	faces::Face _currentFacing;

	int _horizontalRays, _verticalRays;

	bool _player_visible;
};


#endif ENEMY_H