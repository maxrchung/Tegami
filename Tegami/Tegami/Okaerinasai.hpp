#pragma once

#include "BoundingRectangle.hpp"
#include "PNGWrapper.hpp"
#include "Time.hpp"
#include "Vector2.hpp"
#include "Utility.hpp"

class Okaerinasai {
public:
	Okaerinasai(Utility* utility);
private:
	// Convert to PNG coordinates
	Color GetFromPNG(const PNGWrapper& bg, Vector2 point) const;

	// Shuffle a vector of points
	// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
	void fisherYates(std::vector<Vector2>& points);

	// Generate points separated by a minimum space
	// Draws based on screen size (853,480) and converts to SB coordinates
	// http://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
	std::vector<Vector2> poissonDisk(float minDist, int maxTries = 30) const;
	
	Utility* utility;
};
