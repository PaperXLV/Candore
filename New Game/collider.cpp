#include "collider.h"
typedef std::map<std::vector<int>, int>::iterator it_map;

Collider::Collider() {}

Collider::Collider(Vector2 ownerPos, Rectangle pos) :
	_position(pos),
	collidingLeft(false),
	collidingRight(false),
	collidingTop(false),
	collidingBottom(false)
{
	this->_childDist = Vector2(this->_position.getLeft() - ownerPos.x, this->_position.getTop() - ownerPos.y);
}

void Collider::generateCorners(std::map<std::vector<int>, int> levelMap) {
	//cheating level_constants::BLOCK_TO_PIXEL_SCALE
	std::vector<int> vabove{ this->_position.getLeft() / 32, (this->_position.getTop() / 32) - 1 };
	std::vector<int> vright{ (this->_position.getLeft() / 32) + 1, this->_position.getTop() / 32 };
	std::vector<int> vleft{ (this->_position.getLeft() / 32) - 1, this->_position.getTop() / 32 };
	std::vector<int> vbelow{ this->_position.getLeft() / 32, (this->_position.getTop() / 32) + 1 };
	//might have to add try catch handlers
	bool above = (levelMap[vabove] == 1 || levelMap[vabove] == 2);
	bool right = (levelMap[vright] == 1 || levelMap[vright] == 2);
	bool left = (levelMap[vleft] == 1 || levelMap[vleft] == 2);
	bool below = (levelMap[vbelow] == 1 || levelMap[vbelow] == 2);

	if (!above) {
		if (!right) {
			this->_corners.push_back(Vector2(this->_position.getRight(), this->_position.getTop()));
		}
		if (!left) {
			this->_corners.push_back(Vector2(this->_position.getLeft(), this->_position.getTop()));
		}
	}
	if (!below) {
		if (!right) {
			this->_corners.push_back(Vector2(this->_position.getRight(), this->_position.getBottom()));
		}
		if (!left) {
			this->_corners.push_back(Vector2(this->_position.getLeft(), this->_position.getBottom()));
		}
	}

	if (above) {
		if (right) {
			this->_corners.push_back(Vector2(this->_position.getRight(), this->_position.getTop()));
		}
		if (left) {
			this->_corners.push_back(Vector2(this->_position.getLeft(), this->_position.getTop()));
		}
	}
	if (below) {
		if (right) {
			this->_corners.push_back(Vector2(this->_position.getRight(), this->_position.getBottom()));
		}
		if (left) {
			this->_corners.push_back(Vector2(this->_position.getLeft(), this->_position.getBottom()));
		}
	}
}

void Collider::update(Vector2 ownerPos) {
	this->_position.setXYCoord(ownerPos.x + this->_childDist.x, ownerPos.y + this->_childDist.y);
}

bool Collider::is_colliding(Collider &other) const {
	Rectangle otherrect = other.getPosition();
	Rectangle rect = this->getPosition();
	//inside on right, inside on top, inside on bottom, inside on left
	//Check every point
	return
		otherrect.is_colliding(Vector2(rect.getLeft(), rect.getTop())) ||
		otherrect.is_colliding(Vector2(rect.getRight(), rect.getTop())) ||
		otherrect.is_colliding(Vector2(rect.getRight(), rect.getBottom())) ||
		otherrect.is_colliding(Vector2(rect.getLeft(), rect.getBottom()));
}

void Collider::resetCollisions() {
	this->collidingLeft = false;
	this->collidingRight = false;
	this->collidingBottom = false;
	this->collidingTop = false;
}