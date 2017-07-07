#pragma once

#define PI 3.14159265358979

class Utility
{
public:
	Utility();
	float barTimeStep = 3117;
	float quarterTimeStep = barTimeStep / 4; //milliseconds
	float eigthTimeStep = quarterTimeStep / 2;

};

