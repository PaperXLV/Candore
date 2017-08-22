#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "sprite.h"

class Background : public Sprite {
public:
	Background() {}
	Background(Graphics &graphics, const std::string &filepath) :
		Sprite(graphics, filepath, 0, 0, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT, Vector2(0, 0)) 
	{
		//the ol switcheroo to get the SDL_TEXTUREACCESS_TARGET flag on the spritesheet
		SDL_Texture *targetable = SDL_CreateTexture(graphics.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT);
		SDL_SetTextureBlendMode(targetable, SDL_BLENDMODE_NONE);
		SDL_SetRenderTarget(graphics.getRenderer(), targetable);
		SDL_RenderCopy(graphics.getRenderer(), this->_spriteSheet, NULL, NULL);
		SDL_DestroyTexture(this->_spriteSheet);
		this->_spriteSheet = targetable;
		//create 2nd copy to return to in update
		this->_original = SDL_CreateTexture(graphics.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT);
		SDL_SetTextureBlendMode(this->_original, SDL_BLENDMODE_NONE);
		SDL_SetRenderTarget(graphics.getRenderer(), this->_original);
		SDL_RenderCopy(graphics.getRenderer(), this->_spriteSheet, NULL, NULL);
		SDL_SetRenderTarget(graphics.getRenderer(), NULL);
	}
	~Background() {}

	void setTexture(SDL_Texture *newtex) {
		SDL_DestroyTexture(this->_spriteSheet);
		this->_spriteSheet = newtex;
	}

	void update(Graphics &graphics) {
		SDL_DestroyTexture(this->_spriteSheet);
		this->_spriteSheet = SDL_CreateTexture(graphics.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, globalsettings.SCREEN_WIDTH, globalsettings.SCREEN_HEIGHT);
		SDL_SetTextureBlendMode(this->_spriteSheet, SDL_BLENDMODE_NONE);
		SDL_SetRenderTarget(graphics.getRenderer(), this->_spriteSheet);
		SDL_RenderCopy(graphics.getRenderer(), this->_original, NULL, NULL);
		SDL_SetRenderTarget(graphics.getRenderer(), NULL);
	}

	void draw(Graphics &graphics) {
		Sprite::draw(graphics, this->_position.x, this->_position.y);
		//Manual color keying


		this->update(graphics);
	}
	
private:
	SDL_Texture *_original;
};

#endif BACKGROUND_H