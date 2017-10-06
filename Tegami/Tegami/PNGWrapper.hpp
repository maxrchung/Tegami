#pragma once

#include "Color.hpp"
#include "png.h"
#include <string>

// All referenced from http://zarb.org/~gc/html/libpng.html
// I should stop probably script kiddying o wello
class PNGWrapper {
public:
	PNGWrapper(const std::string& path);
	Color Get(int x, int y) const;
	png_structp png;
	png_infop info;
	int width;
	int height;
	std::vector<std::vector<Color>> grid;
};