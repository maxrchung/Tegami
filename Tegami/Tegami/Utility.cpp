#include "Utility.hpp"

Utility::Utility() {
}

Vector2 Utility::panelToOsu(Vector2 panelCoordinates) {
	float convertedX = (panelCoordinates.x - panelMidPoint.x + panelPosition.x) * panelToOsuScaling.x;
	float convertedY = (-panelCoordinates.y + panelMidPoint.y - panelPosition.y) * panelToOsuScaling.y;

	Vector2 osuCoordinate(convertedX, convertedY);
	return osuCoordinate;
}
