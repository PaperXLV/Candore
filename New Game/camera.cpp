#include "camera.h"
#include <math.h>

Camera::Camera() {}

Camera::Camera(Vector2 centerscreen) :
	_centerScreen(centerscreen)
{}

Camera::~Camera() {}

void Camera::centerOnObject(Vector2 position) {
	this->_offset.x = round(position.x - this->_centerScreen.x);
	this->_offset.y = round(position.y - this->_centerScreen.y);
}

void Camera::move(Vector2 amount) {
	this->_offset.x += amount.x;
	this->_offset.y += amount.y;
}

Vector2 Camera::getOffset() {
	return this->_offset;
}

void Camera::setOffset(Vector2 newoffset) {
	this->_offset.x = newoffset.x;
	this->_offset.y = newoffset.y;
}