#pragma once

#include "Vector2.hpp"
#include <vector>

class BoundingRectangle {
public:
	BoundingRectangle(std::vector<Vector2> points);
	// Generate a point from within this bounding rect
	Vector2 GeneratePoint();
	bool Contains(Vector2 point);

	float top = -1000;
	float bot = 1000;
	float left = 1000;
	float right = -1000;
	int width = 0;
	int height = 0;
};
