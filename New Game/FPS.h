#pragma once
#ifndef FPS_H
#define FPS_H

#include <iostream>

struct FPS_Display {
	float deltaTime = 0.0f;

	void update(int elapsedTime) {
		deltaTime += (elapsedTime - deltaTime) * 0.1f;
	}

	void displayFPS() {
		float fps = 1.0f / deltaTime;
		std::cout << fps << std::endl;
	}
};





#endif FPS_H