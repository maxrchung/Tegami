#include "Frame.hpp"

Frame::Frame(std::vector<Line> lines, std::vector<Rectangle> rectangles, Time time) 
	: lines(lines), rectangles(rectangles), time(time) {
}
