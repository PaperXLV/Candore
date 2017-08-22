#pragma once
#ifndef GAME_H
#define GAME_H

//includes
#include "graphics.h"
#include "audio.h"
#include "background.h"
#include "input.h"
#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "slime.h"
#include "camera.h"
#include "level.h"
#include "FPS.h"
#include <algorithm>
#include <time.h>
#include <memory>


class Game {
public:
	Game();
	~Game();

private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(int elapsedTime);

	Graphics _graphics;
	Audio _audio;
	Background _background;
	Background _foreground;
	Input _input;
	SDL_Event _event;
	Player _player;
	Camera _camera;
	Level _level;
	FPS_Display _fps;
	//Scene _scene

	std::vector<std::unique_ptr<Enemy>> _enemies;


	template <typename Der>
	void addItem(const Der &addition) {
		this->_enemies.push_back(std::make_unique<Der>(addition));
	}
};



#endif GAME_H