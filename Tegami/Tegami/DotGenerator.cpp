#include "Character.hpp"
#include "DotGenerator.hpp"
#include "DotTrajectory.hpp"
#include "PartGen.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Stroke.hpp"
#include "StrokeManager.hpp"
#include "Time.hpp"

#include "PoissonGenerator.h"

DotGenerator::DotGenerator(Utility *utility, int numberOfDots, float dotScale, float scaleOffset, float dotVelocity, float speedOffset, Color colorBase, int colorOffset, float fadeBase, float fadeOffset, float startTime, float endTime, const std::string& imagePath, float extraStartFade, bool expand, float expandAmount)
{
	PoissonGenerator::DefaultPRNG randomPoint;
	const auto dotCoord = PoissonGenerator::GeneratePoissonPoints(numberOfDots, randomPoint, 30, false);

	int currTime = 0;
	int currTime2 = 0;
	float timeNext;

	for (int i = 0; i < dotCoord.size(); i++) {
		Sprite *dot = new Sprite(imagePath, Vector2(dotCoord[i].x*Vector2::ScreenSize.x - Vector2::ScreenSize.x / 2, dotCoord[i].y*Vector2::ScreenSize.y - Vector2::ScreenSize.y / 2), Layer::Foreground, Origin::Centre);
		currTime = startTime;
		currTime2 = startTime;

		float scale = generateRandom(dotScale, scaleOffset);
		dot->Scale(startTime, startTime + 1, scale, scale);

		Color currColor = generateColor(colorBase, colorOffset);
		for (int j = 0; currTime < endTime; j++) {

			currTime = currTime2;
			currTime2 += utility->quarterTimeStep;

			Color nextColor = generateColor(colorBase, colorOffset);
			dot->Color(currTime, currTime2, currColor, nextColor, Easing::Linear);
			currColor = nextColor;

			if (expand) {
				dot->Scale(currTime - 200, currTime, dotScale * 1, dotScale * expandAmount, Easing::QuadOut);
				dot->Scale(currTime, currTime + 200, dotScale * expandAmount, dotScale * 1, Easing::QuadOut);
			}

		}

		float tau = 2 * PI;
		float angle = fmod(rand(), tau);

		float startX = dotCoord[i].x*Vector2::ScreenSize.x - Vector2::ScreenSize.x / 2;
		float startY = dotCoord[i].y*Vector2::ScreenSize.y - Vector2::ScreenSize.y / 2;

		float time = startTime;

		DotTrajectory coord = box_collision(startX, startY, angle);
		float speed = generateRandom(dotVelocity, speedOffset);
		timeNext = time + coord.distance / speed;
		dot->Move(time, timeNext, startX, startY, coord.x, coord.y, Easing::Linear);

		while (time < endTime) {
			startX = coord.x;
			startY = coord.y;
			time = timeNext;

			coord = box_collision(coord.x, coord.y, coord.angle);
			timeNext = time + coord.distance / speed;
			dot->Move(time, timeNext, startX, startY, coord.x, coord.y, Easing::Linear);
		}

		float fade = generateRandom(fadeBase, fadeOffset);
		dot->Fade(startTime - utility->quarterTimeStep - extraStartFade, startTime, 0, fade, Easing::CubicIn);
		dot->Fade(endTime, endTime + utility->quarterTimeStep, fade, 0, Easing::SineIn);
	}
}

DotTrajectory  DotGenerator::box_collision(float x, float y, float angle) {

	DotTrajectory out;

	//Coordinates of the box
	float topY = Vector2::ScreenSize.y / 2;
	float rightX = Vector2::ScreenSize.x / 2;
	float botY = -Vector2::ScreenSize.y / 2;
	float leftX = -Vector2::ScreenSize.x / 2;

	float sinInput = sin(angle);
	float cosInput = cos(angle);
	float tanInput = tan(angle);
	int quadrant;


	if (sinInput >= 0) {
		if (cosInput >= 0) {
			quadrant = 1;
		}
		else {
			quadrant = 2;
		}
	}
	else {
		if (cosInput >= 0) {
			quadrant = 4;
		}
		else {
			quadrant = 3;
		}
	}

	switch (quadrant) {
	case 1:
		if (((topY - y) / (rightX - x)) <= tanInput) {
			out.x = x + (topY - y) / tanInput;
			out.y = topY;
			out.angle = -angle;
		}
		else {
			out.x = rightX;
			out.y = y + (rightX - x) * tanInput;
			out.angle = PI - angle;
		}
		break;

	case 2:
		if (((topY - y) / (leftX - x)) >= tanInput) {
			out.x = x + (topY - y) / tanInput;
			out.y = topY;
			out.angle = -angle;
		}
		else {
			out.x = leftX;
			out.y = y + (leftX - x) * tanInput;
			out.angle = PI - angle;
		}
		break;

	case 3:
		if (((botY - y) / (leftX - x)) <= tanInput) {
			out.x = x + (botY - y) / tanInput;
			out.y = botY;
			out.angle = -angle;
		}
		else {
			out.x = leftX;
			out.y = y + (leftX - x) * tanInput;
			out.angle = PI - angle;
		}
		break;

	case 4:
		if (((botY - y) / (rightX - x)) >= tanInput) {
			out.x = x + (botY - y) / tanInput;
			out.y = botY;
			out.angle = -angle;

		}
		else {
			out.x = rightX;
			out.y = y + (rightX - x) * tanInput;
			out.angle = PI - angle;
		}
		break;

	default: break;
	}

	out.distance = fabs((out.x - x) * tanInput / sinInput);

	return out;
}

Color DotGenerator::generateColor(Color base, int offset) {
	if (offset == 0) {
		return base;
	}

	auto random = Color(generateColorValue(base.r, offset),
		generateColorValue(base.g, offset),
		generateColorValue(base.b, offset));

	return random;
}

float DotGenerator::generateColorValue(float base, int offset) {
	auto value = rand() % (offset * 2) - offset + base;
	if (value > 255) {
		value = 255;
	}
	else if (value < 0) {
		value = 0;
	}
	return value;
}

float DotGenerator::generateRandom(float base, float offset) {
	if (offset == 0) {
		return base;
	}

	auto adjusted = int(offset * 2 * 1000);
	auto random = (rand() % adjusted / 1000.0f) - offset + base;
	return random;
}