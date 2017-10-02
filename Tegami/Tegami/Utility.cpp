#include "Utility.hpp"

float Utility::DToR(float degrees) {
	float radians = (degrees / 180) * PI;
	return radians;
}