#include "Utility.hpp"

Utility::Utility() {
}

Vector2 Utility::panelToOsuPoint(Vector2 panelCoordinates) {
	float convertedX = (panelCoordinates.x - panelMidPoint.x + panelPosition.x) * panelToOsuScaling.x;
	float convertedY = (-panelCoordinates.y + panelMidPoint.y - panelPosition.y) * panelToOsuScaling.y;

	Vector2 osuCoordinate(convertedX, convertedY);
	return osuCoordinate;
}

Vector2 Utility::panelToOsuSize(Vector2 panelSize) {
	float convertedWidth = panelSize.x * panelToOsuScaling.x;
	float convertedHeight = panelSize.y * panelToOsuScaling.y;

	Vector2 osuSize(convertedWidth, convertedHeight);
	return osuSize;
}

float Utility::DToR(float degrees) {
	float radians = (degrees / 180) * PI;
	return radians;
}