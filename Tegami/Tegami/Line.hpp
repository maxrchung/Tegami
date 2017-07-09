#pragma once

#include "Vector2.hpp"

class Line {
public:
	Line(Vector2 start, Vector2 end);
	Vector2 start;
	Vector2 end;
};