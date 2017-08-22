#include "input.h"

/* Input Class
* Keeps track of keyboard state
*/

void Input::beginNewFrame() {
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
}

void Input::clearHeldKeys() {
	this->_heldKeys.clear();
}

void Input::addHeldKey(SDL_Scancode key) {
	this->_heldKeys[key] = true;
}
/* void keyDownEvent
* called when a key is pressed
*/

void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

//Called when a key is released

void Input::keyUpEvent(const SDL_Event& event) {
	this->_heldKeys[event.key.keysym.scancode] = false;
	this->_releasedKeys[event.key.keysym.scancode] = true;
}


bool Input::wasKeyPressed(std::vector<SDL_Scancode> keys) {
	for (int i = 0; i < keys.size(); i++) {
		if (this->_pressedKeys[keys[i]]) {
			return true;
		}
	}
	return false;
}

bool Input::wasKeyReleased(std::vector<SDL_Scancode> keys) {
	for (int i = 0; i < keys.size(); i++) {
		if (this->_releasedKeys[keys[i]]) {
			return true;
		}
	}
	return false;
}


bool Input::isKeyHeld(std::vector<SDL_Scancode> keys) {
	for (int i = 0; i < keys.size(); i++) {
		if (this->_heldKeys[keys[i]]) {
			return true;
		}
	}
	return false;
}
