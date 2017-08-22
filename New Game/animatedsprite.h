#pragma once
#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "sprite.h"
#include <map>
#include <vector>

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
		Vector2 spawnpos, float timeToUpdate);
	~AnimatedSprite();

	void playAnimation(std::string animation, bool once = false);

	void update(int elapsedTime);
	void draw(Graphics &graphics, float x, float y);
	void drawRotate(Graphics &graphics, float x, float y, double angle, SDL_Point center, SDL_RendererFlip flip);

	void setRect(int x, int y, int width, int height);

protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);
	void resetAnimations();
	void stopAnimation();
	void setVisible(bool visible);

	virtual void animationDone(std::string currentAnimation) = 0;
	virtual void setupAnimations() = 0;

	const inline int getFrameIndex() const { return this->_frameIndex; }

private:
	std::map<std::string, std::vector<SDL_Rect>> _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visible;
};




#endif ANIMATEDSPRITE_H