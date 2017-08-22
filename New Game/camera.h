#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

class Camera {
public:
	Camera();
	Camera(Vector2 offset);
	~Camera();

	void centerOnObject(Vector2 position);
	void move(Vector2 amount);

	Vector2 getOffset();
	void setOffset(Vector2 newoffset);

private:
	Vector2 _offset;
	Vector2 _centerScreen;
};

#endif CAMERA_H