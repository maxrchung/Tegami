#pragma once

#include "DotTrajectory.hpp"
#include "Utility.hpp"
#include "Color.hpp"

#include <vector>

class DotGenerator
{
public:
	//velocity units: pixels/ms or 1000 pixels/s
	DotGenerator(Utility *utility, int numberOfDots, float dotScale, float scaleOffset, float dotVelocity, float speedOffset, Color colorBase, int colorOffset, float fadeBase, float fadeOffset, float startTime, float endTime, const std::string& imagePath, bool expand, float expandAmount = 1.1f);

	// Generates a color from a minimum color and offset amount
	Color generateColor(Color base, int offset);
	// Helper to compute one Color value
	float generateColorValue(float base, int offset);

	// Generates a value based off of a base and variance amount
	// Used for speed, scale, and fade offsets
	float generateRandom(float base, float offset);

private:
	DotTrajectory box_collision(float x, float y, float angle);
};