#pragma once

#include "Vector2.hpp"
#include <string>

#define PI 3.14159265358979

class Utility
{
public:
	Utility();
	float barTimeStep = 3117;
	float quarterTimeStep = barTimeStep / 4; //milliseconds
	float eigthTimeStep = quarterTimeStep / 2;

	std::string blockPath = std::string("sprite/whiteblock.png");

	Vector2 panelPosition = Vector2(-40, -25);
	Vector2 panelScreenSize = Vector2(1380, 820);
	Vector2 panelDrawSize = Vector2(1280, 720);
	Vector2 panelMidPoint = Vector2(panelScreenSize.x / 2 + panelPosition.x, panelScreenSize.y / 2 + panelPosition.y);
	Vector2 panelToOsuScaling = Vector2(Vector2::ScreenSize.x / panelDrawSize.x, Vector2::ScreenSize.y / panelDrawSize.y);
	// Converts a coordiante in panel to osu! - Applies coordinate system/scaling changes
	Vector2 panelToOsu(Vector2 panelCoordinates);
};

