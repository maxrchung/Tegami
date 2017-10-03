#pragma once

#include <vector>
#include "Vector2.hpp"

class Polygon {
public:
	Polygon(std::vector<Vector2> points);
	std::vector<Vector2> points;
};
