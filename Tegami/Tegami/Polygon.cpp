#include "Polygon.hpp"

Polygon::Polygon(std::vector<Vector2> points) 
	: points(points), boundingRect(points) {
}

Vector2 Polygon::GeneratePoint() {
	Vector2 bounded = boundingRect.GeneratePoint();
	while (!pnpoly(bounded)) {
		bounded = boundingRect.GeneratePoint();
	}
	return bounded;
}

bool Polygon::pnpoly(Vector2 test) {
	int i, j, c = 0;
	for (i = 0, j = points.size() - 1; i < points.size(); j = i++) {
		if (((points[i].y > test.y) != (points[j].y > test.y)) &&
			(test.x < (points[j].x - points[i].x) * (test.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
			c = !c;
	}
	return c;
}

bool Polygon::Contains(Vector2 point) {
	if (!boundingRect.Contains(point)) {
		return false;
	}
	return pnpoly(point);
}