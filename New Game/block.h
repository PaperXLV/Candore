#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include "collider.h"
#include "sprite.h"

namespace block_constants {
	const int width = 32;
	const int height = 32;
}


class Block : public Sprite {
public:
	Block() {}
	Block(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, Vector2 spawnpos) :
		Sprite(graphics, filepath, sourceX, sourceY, width, height, spawnpos),
		_blockCollider(spawnpos, Rectangle(spawnpos.x, spawnpos.y, width, height))
	{}

	~Block() {}

	void identifyCorners(std::map<std::vector<int>, int> levelMap) {
		this->_blockCollider.generateCorners(levelMap);
	}

	const Collider getCollider() const { return this->_blockCollider; }

	void update(int elapsedTime) {
		Sprite::update();
	}

	void draw(Graphics &graphics, Vector2 offset) {
		Sprite::draw(graphics, this->_position.x - offset.x, this->_position.y - offset.y);
	}

	void setCollider(const Collider &collider) {
		this->_blockCollider = collider;
	}

private:
	Collider _blockCollider;
};


#endif BLOCK_H