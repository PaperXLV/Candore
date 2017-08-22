#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "rectangle.h"
#include <string>
#include <SDL.h>
#include "graphics.h"

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
		Vector2 spawnpos);

	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, int x, int y);
	void draw(Graphics &graphics, int x, int y, int width, int height);

	const inline float getX() const { return this->_position.x; }
	const inline float getY() const { return this->_position.y; }
	const inline float getW() const { return this->_width; }
	const inline float getH() const { return this->_height; }

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const;

	SDL_Texture *getTexture() { return this->_spriteSheet; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);

	void setDistRectX(int value);
	void setDistRectY(int value);
	void setDistRectW(int value);
	void setDistRectH(int value);

protected:
	Vector2 _position;
	float _width, _height;

	SDL_Texture *_spriteSheet;
	SDL_Rect _sourceRect;

	Rectangle _boundingBox;
	// Rectangle BoundingBox
private:

};



#endif SPRITE_H