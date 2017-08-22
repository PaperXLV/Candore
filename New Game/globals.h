#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#define PI 3.14159265358979f
#include "settings.h"

const Settings globalsettings = Settings();
namespace globals {
	const float gravity = 0.000128f; //128 / 1 million

	static float degtorad(float deg) { return deg * PI / 180; }
}

namespace level_constants {
	const int BLOCK_TO_PIXEL_SCALE = 32;

	const int LEVEL_WIDTH = 100;
	const int LEVEL_HEIGHT = 100;
	const int ROOM_WIDTH_MAX = 10;
	const int ROOM_WIDTH_MIN = 5;
	const int ROOM_HEIGHT_MAX = 7;
	const int ROOM_HEIGHT_MIN = 3;
	const int CORRIDOR_MIN = 10;
	const int CORRIDOR_MAX = 13;

	const int DIST_BETWEEN_LIGHTS = 7;
	const int DIST_OF_LIGHT = 640; //10 * 32
}

namespace sides {
	enum Side {
		RIGHT,
		LEFT,
		TOP,
		BOTTOM,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

namespace faces {
	enum Face {
		RIGHT,
		LEFT,
		NONE
	};
}

namespace direction {
	enum Direction {
		RIGHT,
		LEFT,
		DOWN,
		UP,
		NONE
	};
	static std::string asString(Direction dir) {
		switch (dir) {
		case(0):
			return "RIGHT";
		case(1):
			return "LEFT";
		case(2):
			return "DOWN";
		case(3):
			return "UP";
		case(4):
			return "NONE";
		}
	}
}

//add mas
namespace weapons {
	enum WeaponName {
		Sword,
		NONE
	};
}

struct Vector2 {
	float x, y;

	Vector2() :
		x(0), y(0)
	{}
	Vector2(float x, float y) :
		x(x), y(y)
	{}

	Vector2 multiply(Vector2 init, float scale) {
		return Vector2(init.x * scale, init.y * scale);
	}

	Vector2 zero() {
		return Vector2(0, 0);
	}

	Vector2 right() {
		return Vector2(1.0f, 0);
	}

	Vector2 left() {
		return Vector2(-1.0f, 0);
	}

	Vector2 up() {
		return Vector2(0, 1.0f);
	}

	Vector2 down() {
		return Vector2(0, -1.0f);
	}

	Vector2 one() {
		return Vector2(1.0f, 1.0f);
	}
};


static bool operator==(const Vector2 &other, const Vector2 &first) {
	return (first.x == other.x && first.y == other.y);
}

#endif GLOBALS_H