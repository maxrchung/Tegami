#pragma once

#include "Line.hpp"
#include "Rectangle.hpp"
#include "Time.hpp"

#include <vector>

class Frame {
public:
	Frame(std::vector<Line> lines, std::vector<Rectangle> rectangles, Time time);
	std::vector<Line> lines;
	std::vector<Rectangle> rectangles;
	Time time;
};
