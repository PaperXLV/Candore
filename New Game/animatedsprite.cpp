#include "animatedsprite.h"


AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnpos, float timeToUpdate) :
	Sprite(graphics, filepath, sourceX, sourceY, width, height, spawnpos),
	_frameIndex(0),
	_timeElapsed(0),
	_visible(true),
	_timeToUpdate(timeToUpdate),
	_currentAnimationOnce(false),
	_currentAnimation("Idle")
{ }

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations() {
	this->_animations.clear();
	this->_offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->_currentAnimation = once;
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible) {
	this->_visible = visible;
}

void AnimatedSprite::stopAnimation() {
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::setRect(int x, int y, int width, int height) {
	this->_position.x = x;
	this->_position.y = y;
}


void AnimatedSprite::update(int elapsedTime) {
	Sprite::update();

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed >= this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
			this->_frameIndex++;
		}
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

void AnimatedSprite::draw(Graphics &graphics, float x, float y) {
	if (this->_visible) {
		if (x + this->_sourceRect.w >= 0 && y + this->_sourceRect.h >= 0 && x <= globalsettings.SCREEN_WIDTH && y <= globalsettings.SCREEN_HEIGHT) {
			SDL_Rect destinationRect;
			destinationRect.x = x + this->_offsets[this->_currentAnimation].x;
			destinationRect.y = y + this->_offsets[this->_currentAnimation].y;
			destinationRect.w = this->_sourceRect.w;
			destinationRect.h = this->_sourceRect.h;

			SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
			graphics.blitSurfacer(this->_spriteSheet, &sourceRect, &destinationRect);
		}
	}
}

void AnimatedSprite::drawRotate(Graphics &graphics, float x, float y, double angle, SDL_Point center, SDL_RendererFlip flip) {
	if (this->_visible) {
		if (x + this->_sourceRect.w >= 0 && y + this->_sourceRect.h >= 0 && x <= globalsettings.SCREEN_WIDTH && y <= globalsettings.SCREEN_HEIGHT) {
			SDL_Rect destinationRect;
			destinationRect.x = x + this->_offsets[this->_currentAnimation].x;
			destinationRect.y = y + this->_offsets[this->_currentAnimation].y;
			destinationRect.w = this->_sourceRect.w;
			destinationRect.h = this->_sourceRect.h;

			SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
			graphics.blitSurfaceRotate(this->_spriteSheet, &sourceRect, &destinationRect, angle, center, flip);
		}
	}
}

