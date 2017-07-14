#pragma once

#include "Sprite.hpp"
#include <string>

// Used in StrokeAnimation for drawing lines/objects frame by frame
// Saves space so you don't have to keep remaking sprites
class SpritePool {
public:
	SpritePool(std::string imagePath, Origin origin);
	Sprite* Get(int index);
	std::vector<Sprite*> sprites;
private:
	std::string imagePath;
	Origin origin;
};