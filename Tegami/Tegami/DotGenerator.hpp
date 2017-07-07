#pragma once

#include "DotTrajectory.hpp"
#include "Utility.hpp"

#include <vector>

class DotGenerator
{
public:
	//velocity units: pixels/ms or 1000 pixels/s
	DotGenerator(Utility *utility, int numberOfDots, float dotScale, float dotVelocity, float startTime, float endTime);

private:
	DotTrajectory box_collision(float x, float y, float angle);
};

