#pragma once

#include "BoundingRectangle.hpp"
#include "Time.hpp"
#include "Vector2.hpp"
#include "Utility.hpp"

class Okaerinasai {
public:
	Okaerinasai(Utility* utility);
private:
	// Convert point from osu! to storyboard coordinates
	Vector2 fromOsu(float x, float y);

	// Shuffle a vector of points
	// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
	void fisherYates(std::vector<Vector2>& points);

	// Generates points separated by a given minimum space in a given space 
	// http://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
	std::vector<Vector2> poissonDisk(float minDist, int maxTries = 30);

	Utility* utility;
};
