#pragma once

#include "Vector2.hpp"

class Rectangle {
public:
	Rectangle(Vector2 position, Vector2 size, float rotation);
	Vector2 position;
	Vector2 center;
	Vector2 size;
	float rotation;
};