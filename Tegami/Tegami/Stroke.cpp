#include "Stroke.hpp"

Stroke::Stroke(std::vector<Vector2> points)
	: points(points) {
}

//Stroke Stroke::operator*(float a) {
//	std::vector<Vector2> out{ points.size() };
//	for (int i = 0; i < points.size(); i++) {
//		out[i] = points[i] * a;
//	}
//	return Stroke(out);
//}

//Stroke Stroke::operator=(Stroke a) {
//	return a;
//}