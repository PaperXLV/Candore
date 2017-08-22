#include "sprite.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnpos) :
	_position(spawnpos),
	_width(width),
	_height(height)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.h = height;
	this->_sourceRect.w = width;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filepath));	

	if (this->_spriteSheet == NULL) {
		printf("Sprite could not be loaded");
	}

	this->_boundingBox = Rectangle(this->_position.x, this->_position.y, width, height);
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, int x, int y) {
	if (x + this->_sourceRect.w >= 0 && y + this->_sourceRect.h >= 0 && x <= globalsettings.SCREEN_WIDTH && y <= globalsettings.SCREEN_HEIGHT) {
		SDL_Rect destinationRect = { x, y, this->_sourceRect.w, this->_sourceRect.h };
		graphics.blitSurfacer(this->_spriteSheet, &this->_sourceRect, &destinationRect);
	}
}

void Sprite::draw(Graphics &graphics, int x, int y, int width, int height) {
	if (x + width >= 0 && y + height >= 0 && x <= globalsettings.SCREEN_WIDTH && y <= globalsettings.SCREEN_HEIGHT) {
		SDL_Rect destinationRect = { x, y, width, height };
		graphics.blitSurfacer(this->_spriteSheet, &this->_sourceRect, &destinationRect);
	}
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_position.x, this->_position.y, this->_sourceRect.w, this->_sourceRect.h);
}

const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

//Side getCollisionSide
//Deterine which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}







void Sprite::setSourceRectX(int value) {
	this->_sourceRect.x = value;
}
void Sprite::setSourceRectY(int value) {
	this->_sourceRect.y = value;
}
void Sprite::setSourceRectW(int value) {
	this->_sourceRect.w = value;
}
void Sprite::setSourceRectH(int value) {
	this->_sourceRect.h = value;
}


void Sprite::setDistRectX(int value) {
	this->_position.x = value;
}
void Sprite::setDistRectY(int value) {
	this->_position.y = value;
}
void Sprite::setDistRectW(int value) {
	this->_width = value;
}
void Sprite::setDistRectH(int value) {
	this->_height = value;
}