#include "enemy.h"

Enemy::Enemy() {}

Enemy::Enemy(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnpos, int health) :
	AnimatedSprite(graphics, filepath, sourceX, sourceY, width, height, spawnpos, 100),
	_collider(spawnpos, Rectangle(spawnpos.x, spawnpos.y, width, height)),
	_velocity(Vector2(0, 0)),
	_verticalRays(this->_collider.getPosition().getWidth() / ray_constants::distBetweenRays),
	_horizontalRays(this->_collider.getPosition().getHeight() / ray_constants::distBetweenRays),
	_currentHealth(health),
	_maxHealth(health),
	_currentFacing(faces::LEFT)
{
	this->setupAnimations();
}

Enemy::~Enemy() {}

void Enemy::setupAnimations() {}

void Enemy::animationDone(std::string currentAnimation) {}

const float Enemy::getX() const { return this->_position.x; }
const float Enemy::getY() const { return this->_position.y; }
const float Enemy::getCenterX() const { return (this->_position.x + this->_collider.getPosition().getWidth()) / 2; }
const float Enemy::getCenterY() const { return (this->_position.y + this->_collider.getPosition().getHeight()) / 2; }

float Enemy::verticalCollisions() {
	float rayLength = std::abs(this->_velocity.y);
	float directionY = (this->_velocity.y >= 0) ? 1 : -1;
	int collisions = 0;
	for (int i = 3; i < this->_verticalRays - 3; i++) {
		Vector2 rayOrigin = (directionY == -1) ? Vector2(this->_collider.getPosition().getLeft(), this->_collider.getPosition().getTop()) :
			Vector2(this->_collider.getPosition().getLeft(), this->_collider.getPosition().getBottom());
		Vector2 fixedOrigin = Vector2(rayOrigin.x + ((ray_constants::distBetweenRays * i)), rayOrigin.y);

		Raycast hit = Raycast(fixedOrigin, Vector2(0, directionY), rayLength);

		for (int j = 0; j < this->_nearby.size(); j++) {
			if (hit.is_colliding(this->_nearby[j])) {
				//if (std::abs(hit.getDistanceY()) < 16) { // check for if its bein a silly and thinking the above block is supposed to be it
				collisions++;
				rayLength = hit.getDistanceY();
				this->_velocity.y = 0;

				this->_nearby.erase(this->_nearby.begin() + j);

				(directionY == -1) ? this->_collider.collidingTop = true : this->_collider.collidingBottom = true;
				//}
			}
		}
	}
	//YEAH BOIS I CALL THIS ONE THE WHOLE MAKE IT WORK AS IT SHOULD MOVE
	if (collisions == 0) {
		return 0;
	}
	else {
		return rayLength;
	}
}

float Enemy::horizontalCollisions() {
	float rayLength = std::abs(this->_velocity.x);
	float directionX = (this->_velocity.x >= 0) ? 1 : -1;
	int collisions = 0;
	for (int i = 0; i < this->_horizontalRays; i++) {
		Vector2 rayOrigin = (directionX == -1) ? Vector2(this->_collider.getPosition().getLeft(), this->_collider.getPosition().getTop()) :
			Vector2(this->_collider.getPosition().getRight(), this->_collider.getPosition().getTop());
		Vector2 fixedOrigin = Vector2(rayOrigin.x, rayOrigin.y + ((ray_constants::distBetweenRays * i) + this->_velocity.y));

		Raycast hit = Raycast(fixedOrigin, Vector2(directionX, 0), rayLength);
		for (int j = 0; j < this->_nearby.size(); j++) {
			if (hit.is_colliding(this->_nearby[j])) {
				collisions++;
				rayLength = hit.getDistanceX();
				this->_velocity.x = 0;
				(directionX == -1) ? this->_collider.collidingLeft = true : this->_collider.collidingRight = true;
			}
		}
	}
	//YEAH BOIS I CALL THIS ONE THE WHOLE MAKE IT WORK AS IT SHOULD MOVE
	if (collisions == 0) {
		return 0;
	}
	else {
		return rayLength;
	}
}

bool Enemy::checkSightOfPlayer(Collider &playerCollider) {
	Rectangle playerRect = playerCollider.getPosition();
	std::vector<Vector2> playerRectPoints = { Vector2(playerRect.getLeft(), playerRect.getTop()), Vector2(playerRect.getRight(), playerRect.getTop()), 
		Vector2(playerRect.getLeft(), playerRect.getBottom()), Vector2(playerRect.getRight(), playerRect.getBottom()) };

	//create a line between each point, if doesn't run into any other colliders found on _nearby, este es bueno
	for (int i = 0; i < playerRectPoints.size(); i++) {
		Raycast newRay = Raycast(Vector2(this->_collider.getPosition().getCenterX(), this->_collider.getPosition().getCenterY()), playerRectPoints[i]);
		newRay.stopOnCollision(this->_nearby); //so probably gonna need to run through a larger vector of colliders cause this is only the nearby ones

		if (newRay.getStop() == playerRectPoints[i]) {
			printf("LINEOFSIGHT-ESTABLISHED\n");
			return true;
		}
	}
	printf("WE AINT GOT NO SIGHT\n");
	return false;
}

void Enemy::damaged(int damage) {
	this->_currentHealth -= damage;
}

bool Enemy::alive() {
	return this->_currentHealth > 0;
}

void Enemy::update(int elapsedTime, Level &level) {
	this->_velocity.y += globals::gravity;

	this->_nearby = level.getNeighborColliders(this->_collider.getPosition().getCenterX() / level_constants::BLOCK_TO_PIXEL_SCALE, this->_collider.getPosition().getCenterY() / level_constants::BLOCK_TO_PIXEL_SCALE);
	float vertRay = this->verticalCollisions();
	float horizRay = this->horizontalCollisions();

	float prevPosx = this->_position.x;

	(horizRay >= 0) ? this->_position.x += horizRay + (this->_velocity.x * elapsedTime) : this->_position.x += horizRay - (this->_velocity.x * elapsedTime);
	(vertRay >= 0) ? this->_position.y += vertRay + (this->_velocity.y * elapsedTime) : this->_position.y += vertRay - (this->_velocity.y * elapsedTime);

	//update collision and stuff
	/*
	for (int i = 0; i < playerSlashes.size(); i++) {
		if (this->_collider.is_colliding(playerSlashes[i])) {
			//don't hard code damage, read from player's weapon

			//Solution to continuous damage taking
			//When takes damage, play damaged animation
			//Unable to take damage during animation!
			this->damaged(1);
		}
	}
	*/

	this->_collider.update(this->_position);
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics, Vector2 offset) {
	AnimatedSprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
}