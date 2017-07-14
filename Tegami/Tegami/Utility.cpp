#include "Utility.hpp"

Utility::Utility() {
}

float Utility::DToR(float degrees) {
	float radians = (degrees / 180) * PI;
	return radians;
}