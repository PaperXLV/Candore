#include "player.h"
#include <iostream>
#include <algorithm>
#include <numeric>

namespace Player_Constants {
	float jumpspeed = std::sqrt(64 * globals::gravity); //1 block
	float bouncespeed = std::sqrt(896 * globals::gravity); //14 block
	const int collision_segments = 15;
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnpoint) :
	AnimatedSprite(graphics, "Assets/sprites/animated/player/player_new.png", 0, 0, 16, 16, spawnpoint, 100),
	_velocity(Vector2(0, 0)),
	_maxHealth(10),
	_currentHealth(10),
	_currentRotationDeg(0),
	_jumping(false),
	_bounce(false),
	_movingLeft(false),
	_movingRight(false),
	_disableGrounded(false),
	_weapon(graphics, spawnpoint),
	_collider(this->_position, Rectangle(this->_position.x, this->_position.y, 16, 16)),
	_ms_to_deg(0.36f), //360 deg in 1000 ms (1 sec)
	_ms_to_dist(0.064f), //16 px in 250 ms (1/4 sec)
	_rotationRect(this->_position, Vector2(this->_position.x + 16, this->_position.y), Vector2(this->_position.x + 16, this->_position.y + 16), Vector2(this->_position.x, this->_position.y + 16), 16, graphics)
{
	graphics.loadImage("Assets/sprites/animated/player/player_new.png");

	this->setupAnimations();
	this->playAnimation("Idle");
}


void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 16, 16, Vector2(0, 0));
}

void Player::preformMovement(Input &input, int elapsedTime) {
	if (input.wasKeyPressed(globalsettings.jump) && this->_collider.collidingBottom) {
		this->jump();
	}
	if (!this->_movingRight && !this->_movingLeft && !this->_collider.collidingBottom) {
		this->aerialMovement(input, elapsedTime);
	}
	else {
		if ((input.wasKeyPressed(globalsettings.left) || input.isKeyHeld(globalsettings.left)) && !this->_movingLeft) {
			this->_movingLeft = true;
		}
		else if ((input.wasKeyPressed(globalsettings.right) || input.isKeyHeld(globalsettings.right)) && !this->_movingRight) {
			this->_movingRight = true;
		}

		this->_collider.resetCollisions();
		if (this->_movingLeft) {
			this->moveLeft(elapsedTime, input.wasKeyPressed(globalsettings.left) || input.isKeyHeld(globalsettings.left));
		}
		else if (this->_movingRight) {
			this->moveRight(elapsedTime, input.wasKeyPressed(globalsettings.right) || input.isKeyHeld(globalsettings.right));
		}
		else {
			this->stop(elapsedTime);
		}

		if (!this->_disableGrounded) {
			this->stayGrounded(elapsedTime);
		}
		
	}
	//apply gravity	
	this->_velocity.y += globals::gravity * elapsedTime;
}

void Player::aerialMovement(Input &input, int elapsedTime) {
	if (input.wasKeyPressed(globalsettings.right) || input.isKeyHeld(globalsettings.right)) {
		this->_currentRotationDeg += elapsedTime * this->_ms_to_deg;
		this->_position.x += elapsedTime * this->_ms_to_dist;
	}
	else if (input.wasKeyPressed(globalsettings.left) || input.isKeyHeld(globalsettings.left)) {
		this->_currentRotationDeg -= elapsedTime * this->_ms_to_deg;
		this->_position.x -= elapsedTime * this->_ms_to_dist;
	}

	if (this->_currentRotationDeg < 0) {
		this->_currentRotationDeg += 360;
	}
	if (this->_currentRotationDeg > 360) {
		this->_currentRotationDeg -= 360;
	}
}


void Player::moveLeft(int elapsedTime, bool holding) {
	int currentSide = std::trunc(this->_currentRotationDeg / 90);
	this->_currentRotationDeg -= elapsedTime * this->_ms_to_deg;
	this->_position.x -= elapsedTime * this->_ms_to_dist;
	
	//fix angle
	if (this->_currentRotationDeg < 0) {
		this->_currentRotationDeg += 360;
	}
	if (this->_currentRotationDeg > 360) {
		this->_currentRotationDeg -= 360;
	}

	//needs to be here so case 0 degrees works
	int nextSide = std::trunc(this->_currentRotationDeg / 90);

	//finished a rotation
	if (currentSide != nextSide && !holding) {
		this->_currentRotationDeg = 90 * (std::trunc(this->_currentRotationDeg / 90) + 1);
		this->_movingLeft = false;
	}
}

void Player::moveRight(int elapsedTime, bool holding) {
	int currentSide = std::trunc(this->_currentRotationDeg / 90);
	this->_currentRotationDeg += elapsedTime * this->_ms_to_deg;
	this->_position.x += elapsedTime * this->_ms_to_dist;
	int nextSide = std::trunc(this->_currentRotationDeg / 90);

	if (this->_currentRotationDeg < 0) {
		this->_currentRotationDeg += 360;
	}
	if (this->_currentRotationDeg > 360) {
		this->_currentRotationDeg -= 360;
	}

	//finished a rotation
	if (currentSide != nextSide && !holding) {
		this->_currentRotationDeg = 90 * std::trunc(this->_currentRotationDeg / 90);
		this->_movingRight = false;
	}
}

void Player::jump() {
	this->_velocity.y = 0;
	this->_velocity.y -= Player_Constants::jumpspeed;
	this->_movingLeft = false;
	this->_movingRight = false;
	this->_collider.collidingBottom = false;
	this->_jumping = true;
}

void Player::bounce() {
	this->_velocity.y = 0;
	this->_velocity.y -= Player_Constants::bouncespeed;
	this->_movingLeft = false;
	this->_movingRight = false;
	this->_collider.collidingBottom = false;
	this->_jumping = true;
}

void Player::stop(int elapsedTime) {

	float decimal = this->_currentRotationDeg - std::trunc(this->_currentRotationDeg);
	float referenceDegree = static_cast<int>(std::abs(std::trunc(this->_currentRotationDeg))) % 90 + decimal;

	float degree = this->_currentRotationDeg;

	if (referenceDegree < 45 && referenceDegree > 0) {
		//std::cout << "LEFT:";
		this->moveLeft(elapsedTime, false);
	}
	else if (referenceDegree >= 45) {
		this->moveRight(elapsedTime, false);
		//std::cout << "RIGHT:";
	}

	//debug
	//std::cout << "(" << degree << "::" << this->_currentRotationDeg << ")";
}

void Player::stayGrounded(int elapsedTime) {
	//Small bug where if player is on edge of thingy this function doesn't correctly detect the block under and keep em going down
	if (!this->_jumping) {
		Vector2 lowest = this->_rotationRect.getLowestPoint();
		Vector2 middle = this->_rotationRect.getMiddleOfLowestSide();

		Vector2 modified = Vector2(middle.x, middle.y + 10); //test numbers
		int collisions = 0;
		for (int i = 0; i < this->_nearby.size(); i++) {
			Rectangle current = this->_nearby[i].getPosition();
			if (current.is_colliding(modified)) {
				collisions++;
				float movement = this->_nearby[i].getPosition().getTop() - lowest.y;
				this->_position.y += movement;
				this->_collider.collidingBottom = true;
				this->_disableGrounded = false;
			}
		}
		for (int i = 0; i < this->_nearbyBounce.size(); i++) {
			Rectangle current = this->_nearbyBounce[i].getPosition();
			if (current.is_colliding(modified)) {
				collisions++;
				float movement = this->_nearbyBounce[i].getPosition().getTop() - lowest.y;
				if (std::abs(movement) < .002f) movement = 0;
				this->_position.y += movement;
				this->_collider.collidingBottom = true;
				this->_disableGrounded = false;
			}
		}
		if (collisions == 0) {
			this->_disableGrounded = true;
			this->_movingLeft = false;
			this->_movingRight = false;
		}
	}
}

bool Player::is_grounded(int elapsedTime) {
	Vector2 lowest = this->_rotationRect.getPoints()[0];
	for (Vector2 current : this->_rotationRect.getPoints()) {
		if (current.y > lowest.y) lowest.y = current.y;
	}
	Raycast hit = Raycast(lowest, Vector2(0, 1), this->_velocity.y * elapsedTime);
	for (Collider current : this->_nearby) {
		if (hit.collidesWith(current.getPosition())) {
			return true;
		}
	}
	return false;
}

void Player::startFalling() {

}

void Player::animationDone(std::string currentAnimation) {

}

const float Player::getX() const {
	return this->_position.x;
}

const float Player::getY() const {
	return this->_position.y;
}

const float Player::getCenterX() const {
	return this->_rotationRect.getCenter().x;
}

const float Player::getCenterY() const {
	return this->_rotationRect.getCenter().y;
}

const Vector2 Player::getCenter() const {
	return this->_rotationRect.getCenter();
}

Vector2 Player::calculateCollision(int elapsedTime) {
	float rayLengthx = this->_velocity.x * elapsedTime;
	float rayLengthy = this->_velocity.y * elapsedTime;
	float largeMovex = 0;
	float largeMovey = 0;

	//debug
	int bottomCollisions = 0;

	Vector2 result = Vector2(0, 0);

	Vector2 playerCenter = this->_rotationRect.getCenter();

	std::vector<Raycast> playerRays = this->_rotationRect.getRectSides();
	std::vector<Vector2> playerPoints;
	for (int i = 0; i < playerRays.size(); i++) {
		std::vector<Vector2> newPoints = playerRays[i].segment(Player_Constants::collision_segments);
		playerPoints.insert(playerPoints.end(), newPoints.begin(), newPoints.end());
	}

	for (int i = 0; i < playerPoints.size(); i++) {
		float slope;
		try {
			slope = (playerPoints[i].y - playerCenter.y) / (playerPoints[i].x - playerCenter.x);
		}
		catch (...) {
			//only breaks on divide by zero, so yeah we get big number
			slope = 9000;
		}
		if (slope > 1.1 || slope < -1.1) {
			//Point below the center
			if (playerPoints[i].y > playerCenter.y) {
				if (!this->_jumping) {
					//if system works look into optimizing by using this loop now instead of relooping
					Raycast hit = Raycast(playerPoints[i], Vector2(0, 1), rayLengthy);
					for (int j = 0; j < this->_nearby.size(); j++) {
						if (hit.is_colliding(this->_nearby[j])) {
							bottomCollisions++;
							if (std::abs(hit.getDistanceY()) > std::abs(largeMovey)) {
								largeMovey = hit.getDistanceY();
								rayLengthy = hit.getDistanceY();
								result.y = rayLengthy;
							}
							this->_velocity.y = 0;
							this->_collider.collidingBottom = true;
						}
					}
					//go through bounce
					for (int j = 0; j < this->_nearbyBounce.size(); j++) {
						if (hit.is_colliding(this->_nearbyBounce[j])) {
							bottomCollisions++;
							this->_bounce = true;
						}
					}
				}
			}
			else {
				Raycast hit = Raycast(playerPoints[i], Vector2(0, -1), rayLengthy);
				for (int j = 0; j < this->_nearby.size(); j++) {
					if (hit.is_colliding(this->_nearby[j])) {
						if (std::abs(hit.getDistanceY()) > std::abs(largeMovey)) {
							largeMovey = hit.getDistanceY();
							rayLengthy = hit.getDistanceY();
							result.y = rayLengthy;
						}
						if (this->_velocity.y < 0) {
							this->_velocity.y = 0;
						}
						this->_collider.collidingTop = true;
					}
				}
			}
		}
		else if (slope < .9 && slope > -.9) {
			//Point right to the center
			if (playerPoints[i].x > playerCenter.x) {
				Raycast hit = Raycast(playerPoints[i], Vector2(1, 0), rayLengthx);
				for (int j = 0; j < this->_nearby.size(); j++) {
					if (hit.is_colliding(this->_nearby[j])) {
						if (std::abs(hit.getDistanceX()) > std::abs(largeMovex)) {
							largeMovex = hit.getDistanceX();
							rayLengthx = hit.getDistanceX();
							result.x = rayLengthx;
						}
						this->_velocity.x = 0;
						this->_collider.collidingRight = true;
					}
				}
			}
			else {
				Raycast hit = Raycast(playerPoints[i], Vector2(-1, 0), rayLengthx);
				for (int j = 0; j < this->_nearby.size(); j++) {
					if (hit.is_colliding(this->_nearby[j])) {
						if (std::abs(hit.getDistanceX()) > std::abs(largeMovex)) {
							largeMovex = hit.getDistanceX();
							rayLengthx = hit.getDistanceX();
							result.x = rayLengthx;
						}
						this->_velocity.x = 0;
						this->_collider.collidingLeft = true;
					}
				}
			}
		}
	}

	return result;
}

bool Player::alive() {
	return this->_currentHealth > 0;
}

void Player::update(int elapsedTime, Level &level, Input &input) {
	this->preformMovement(input, elapsedTime);

	Vector2 center = this->_rotationRect.getCenter();
	//TODO:: Optimize nearby and nearbyBounce to run in same function, only loop through 1nce
	this->_nearby = level.getNeighborColliders(center.x / level_constants::BLOCK_TO_PIXEL_SCALE, center.y / level_constants::BLOCK_TO_PIXEL_SCALE);
	this->_nearbyBounce = level.getNeighborBounceColliders(center.x / level_constants::BLOCK_TO_PIXEL_SCALE, center.y / level_constants::BLOCK_TO_PIXEL_SCALE);

	Vector2 collisionAdjustments = this->calculateCollision(elapsedTime);

	float horizRay = collisionAdjustments.x;
	(horizRay >= 0) ? this->_position.x += horizRay + (this->_velocity.x * elapsedTime) : this->_position.x += horizRay - (this->_velocity.x * elapsedTime);

	float vertRay = collisionAdjustments.y;
	if (std::abs(vertRay) < .002f) vertRay = 0; //if there are problems with the y value being weird, look here first
	(vertRay >= 0) ? this->_position.y += vertRay + (this->_velocity.y * elapsedTime) : this->_position.y += vertRay - (this->_velocity.y * elapsedTime);

	if (this->_bounce) {
		this->bounce();
	}


	//if (this->_collider.collidingTop) this->_position.y += 1;

	this->_jumping = false;
	this->_bounce = false;

	if (this->_collider.collidingBottom) {
		this->_disableGrounded = false;
	}

	//update collision and stuff
	this->_rotationRect.update(Vector2(this->_position.x + (this->_boundingBox.getWidth() / 2), this->_position.y + (this->_boundingBox.getHeight() / 2)), this->_currentRotationDeg);
	
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics, Vector2 offset) {
	//Debug
	this->_rotationRect.draw(graphics, offset);

	SDL_Point center = SDL_Point { this->_boundingBox.getWidth() / 2, this->_boundingBox.getHeight() / 2 };
	AnimatedSprite::drawRotate(graphics, this->_position.x - offset.x, this->_position.y - offset.y, this->_currentRotationDeg, center, SDL_FLIP_NONE);
	this->_weapon.draw(graphics, this->_position, offset, center, this->_currentRotationDeg);
}






/*
//////////////////////////WEAPON CLASS\\\\\\\\\\\\\\\\\\\\\\\
*/


Weapon::Weapon() {}

Weapon::Weapon(Graphics &graphics, Vector2 spawn) :
	AnimatedSprite(graphics, "Assets/sprites/animated/player/Player_Hit.png", 0, 0, 16, 16, spawn, 100)
{
	graphics.loadImage("Assets/sprites/animated/player/Player_Hit.png");


	this->setupAnimations();
	this->playAnimation("Idle");
}

Weapon::~Weapon() {}

void Weapon::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 16, 16, Vector2(0, 0));
}

void Weapon::animationDone(std::string currentAnimation) {}

void Weapon::draw(Graphics &graphics, Vector2 point, Vector2 offset, SDL_Point center, float rotation) {
	this->_position = point;
	AnimatedSprite::drawRotate(graphics, point.x - offset.x, point.y - offset.y, rotation, center, SDL_FLIP_NONE);
}