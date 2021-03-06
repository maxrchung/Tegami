#pragma once

#include "Vector2.hpp"
#include "Color.hpp"
#include <string>

#define PI 3.14159265358979

class Utility {
public:
	Utility() {};
	float barTimeStep = 3117;
	float quarterTimeStep = barTimeStep / 4; //milliseconds
	float eigthTimeStep = quarterTimeStep / 2;

	float bpm = 77;
	float mpb = 1 / bpm;
	float spb = mpb * 60;
	float mspb = spb * 1000;
	float mspf = mspb / 4;

	Color pinkBg = Color(250, 180, 198);
	Color blueBg = Color(183, 251, 255);

	// Convert degrees to radians
	float DToR(float degrees);
};

