#pragma once

#include "Polygon.hpp"
#include "Time.hpp"
#include "Vector2.hpp"
#include "Utility.hpp"

class Okaerinasai {
public:
	Okaerinasai(Utility* utility);
private:
	// Convert point from osu! to storyboard coordinates
	Vector2 fromOsu(float x, float y);

	Utility* utility;
	Vector2 mid;
	Polygon path;
	Polygon plants;
	Polygon flowers;
	Polygon clouds;
	Polygon leftMountains;
	Polygon rightMountains;
};
