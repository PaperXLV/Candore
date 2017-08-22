#pragma once
#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>
#include <vector>


class Input {
public:
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& event);
	void keyDownEvent(const SDL_Event& event);
	void clearHeldKeys();
	void addHeldKey(SDL_Scancode key);

	bool wasKeyPressed(std::vector<SDL_Scancode> keys);
	bool isKeyHeld(std::vector<SDL_Scancode> keys);
	bool wasKeyReleased(std::vector<SDL_Scancode> keys);
private:
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;
};




#endif INPUT_H