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

	// Offset of actual video
	float videoOffset = 37753;
	Time offsetVideoTime(Time globalTime);

	// Referenced from StrokeAnimation project
	Vector2 panelPosition = Vector2(11, 26);
	Vector2 panelScreenSize = Vector2(1380, 820);
	Vector2 panelDrawSize = Vector2(1280, 720);
	Vector2 panelMidPoint = Vector2(panelScreenSize.x / 2 + panelPosition.x, panelScreenSize.y / 2 + panelPosition.y);
	Vector2 panelToOsuScaling = Vector2(Vector2::ScreenSize.x / panelDrawSize.x, Vector2::ScreenSize.y / panelDrawSize.y);
	// Converts a coordiante in panel to osu! - Applies coordinate system/scaling changes
	Vector2 panelToOsuPoint(Vector2 panelCoordinates);
	Vector2 panelToOsuSize(Vector2 panelSize);

	Utility* utility;
};