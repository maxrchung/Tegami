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

DotGenerator::DotGenerator(Utility *utility, int numberOfDots, float dotScale, float dotVelocity, float startTime, float endTime)
{
	std::vector<Sprite*> dots;
	int rcolor;
	int gcolor;
	int bcolor;

	int gcolor2;
	int rcolor2;
	int bcolor2;

	PoissonGenerator::DefaultPRNG randomPoint;

	int currTime = 0;
	int currTime2 = 0;
	float timeNext;

	//dw about 100000
	const auto dotCoord = PoissonGenerator::GeneratePoissonPoints(numberOfDots, randomPoint, 30, false);

	for (int i = 0; i < dotCoord.size(); i++) {
		Sprite *dot = new Sprite("sprite/lildot.png", Vector2(dotCoord[i].x*Vector2::ScreenSize.x - Vector2::ScreenSize.x / 2, dotCoord[i].y*Vector2::ScreenSize.y - Vector2::ScreenSize.y / 2), Layer::Foreground, Origin::Centre);
		currTime = startTime;
		currTime2 = startTime;
		dot->Scale(startTime, startTime + 1, dotScale, dotScale);
		gcolor = rand() % 255;
		rcolor = rand() % (gcolor + 1);
		bcolor = rand() % 10 + 245;

		int sign = rand() % 2;

		dotVelocity += (pow(-1, sign)*fmod(rand(), 0.02));

		for (int j = 0; currTime < endTime; j++) {

			currTime = currTime2;
			currTime2 += utility->quarterTimeStep;

			gcolor2 = rand() % 255;
			rcolor2 = rand() % (gcolor2 + 1);
			bcolor2 = rand() % 10 + 245;

			dot->Color(currTime, currTime2, Color(rcolor, gcolor, bcolor), Color(rcolor2, gcolor2, bcolor2), Easing::Linear);

			dot->Scale(currTime - 200, currTime, dotScale * 1, dotScale*1.1, Easing::QuadOut);
			dot->Scale(currTime, currTime + 200, dotScale*1.1, dotScale * 1, Easing::QuadOut);

			rcolor = rcolor2;
			gcolor = gcolor2;
			bcolor = bcolor2;
		}

		float tau = 2 * PI;
		float angle = fmod(rand(), tau);

		float startX = dotCoord[i].x*Vector2::ScreenSize.x - Vector2::ScreenSize.x / 2;
		float startY = dotCoord[i].y*Vector2::ScreenSize.y - Vector2::ScreenSize.y / 2;

		float time = startTime;


		DotTrajectory coord = box_collision(startX, startY, angle);
		timeNext = time + coord.distance / (dotVelocity);
		dot->Move(time, timeNext, startX, startY, coord.x, coord.y, Easing::Linear);

		while (time < endTime) {

			startX = coord.x;
			startY = coord.y;
			time = timeNext;

			coord = box_collision(coord.x, coord.y, coord.angle);
			timeNext = time + coord.distance / dotVelocity;
			dot->Move(time, timeNext, startX, startY, coord.x, coord.y, Easing::Linear);
		}

		dot->Fade(startTime - utility->quarterTimeStep, startTime, 0, 0.5, Easing::CubicOut);
		dot->Fade(endTime, endTime + utility->quarterTimeStep, 0.5, 0, Easing::SineIn);
		dots.push_back(dot);
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
			out.angle = angle + PI / 2;
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

