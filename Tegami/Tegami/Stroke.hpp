#pragma once

#include <vector>
#include "Vector2.hpp"

class Stroke {
public:
	Stroke(std::vector<Vector2> points);
	std::vector<Vector2> points;
};