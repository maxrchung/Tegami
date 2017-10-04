#pragma once

#include "BoundingRectangle.hpp"
#include "Vector2.hpp"
#include <vector>

class Polygon {
public:
	Polygon(std::vector<Vector2> points);

	// Generate a point within the polygon
	Vector2 GeneratePoint();
	// https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
	bool pnpoly(Vector2 point);
	bool Contains(Vector2 point);

	BoundingRectangle boundingRect;
	std::vector<Vector2> points;
};
