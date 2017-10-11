#pragma once

#include "Sprite.hpp"
#include <string>

// Used in StrokeAnimation for drawing lines/objects frame by frame
// Saves space so you don't have to keep remaking sprites
class SpritePool {
public:
	SpritePool(std::string imagePath, Origin origin = Origin::Centre);
	Sprite* Get(int index);

	// Gets the next available Sprite
	Sprite* Get();

	// Reset Get counter
	void ReGet();
	std::vector<Sprite*> sprites;
private:
	int getting = 0;
	std::string imagePath;
	Origin origin;
};