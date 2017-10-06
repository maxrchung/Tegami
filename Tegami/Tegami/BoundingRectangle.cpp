#include "BoundingRectangle.hpp"

BoundingRectangle::BoundingRectangle(std::vector<Vector2> points) {
	for (auto& point : points) {
		if (point.x < left) {
			left = point.x;
		}
		else if (point.x > right) {
			right = point.x;
		}

		if (point.y < bot) {
			bot = point.y;
		}
		else if (point.y > top) {
			top = point.y;
		}
	}

	width = right - left;
	height = top - bot;
}

Vector2 BoundingRectangle::GeneratePoint() {
	int x = rand() % width + left;
	int y = rand() % height + bot;
	return Vector2(x, y);
}

bool BoundingRectangle::Contains(Vector2 point) {
	if (point.x < left || point.x > right || point.y < bot || point.y > top) {
		return false;
	}
	return true;
}