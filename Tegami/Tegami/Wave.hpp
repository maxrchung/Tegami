#pragma once

#include "Color.hpp"
class Wave
{
public:
	Wave(float amplitude, float wavelength, float velocity, float timeStart, float timeEnd, Color color, float scale);
	float amplitude;
	float wavelength;
	float velocity;
	float timeStart;
	float timeEnd;
	Color color;
	float scale;
};

