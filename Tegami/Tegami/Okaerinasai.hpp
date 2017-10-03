#pragma once

#include "Polygon.hpp"
#include "Time.hpp"
#include "Vector2.hpp"

class Okaerinasai {
public:
	Okaerinasai();

	Vector2 mid;
private:
	Vector2 convertCoordinate(float x, float y);
};
