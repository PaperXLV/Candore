#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <SDL_scancode.h>




class Settings {
public:
	Settings();
	~Settings();

	void Load();
	void Save();

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	bool is_left(SDL_Scancode key);
	bool is_right(SDL_Scancode key);
	bool is_jump(SDL_Scancode key);

	//Controls
	std::vector<SDL_Scancode> jump;
	std::vector<SDL_Scancode> left;
	std::vector<SDL_Scancode> right;
	std::vector<SDL_Scancode> attack;
};




#endif SETTINGS_H