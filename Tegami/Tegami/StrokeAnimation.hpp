#pragma once

class Utility;
class SpritePool;

#include "Frame.hpp"
#include "Line.hpp"
#include "Rectangle.hpp"
#include "tinyxml2.h"
#include "Vector2.hpp"

#include <string>

using namespace tinyxml2;

class StrokeAnimation {
public:
	StrokeAnimation(Utility* utility, std::string path);
private:
	void drawLines(SpritePool* lines, std::vector<Frame> frames);
	void drawRectangles(SpritePool* rectangles, std::vector<Frame> frames);

	std::vector<Frame> parseFrames(std::string path);
	Line parseLine(tinyxml2::XMLElement* element);
	Vector2 parsePoint(XMLElement* element);
	Vector2 parseSize(XMLElement* element);
	Rectangle parseRectangle(XMLElement* element);

	Utility* utility;
};