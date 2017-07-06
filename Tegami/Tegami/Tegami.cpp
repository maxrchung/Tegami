#include "Character.hpp"
#include "PartGen.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Stroke.hpp"
#include "StrokeManager.hpp"
#include "Time.hpp"

#include "PoissonGenerator.h"

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <random>

#define PI 3.14159265358979

std::vector<float> box_collision(float x, float y, float angle) {

	std::vector <float> out(4);

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
			out[0] = x + (topY - y) / tanInput;
			out[1] = topY;
			out[2] = angle + PI/2;
		}
		else {
			out[0] = rightX;
			out[1] = y + (rightX - x) * tanInput;
			out[2] = PI - angle;
		}
		break;

	case 2:
		if (((topY - y) / (leftX - x)) >= tanInput) {
			out[0] = x + (topY - y) / tanInput;
			out[1] = topY;
			out[2] = -angle;
		}
		else {
			out[0] = leftX;
			out[1] = y + (leftX - x) * tanInput;
			out[2] = PI - angle;
		}
		break;

	case 3:
		if (((botY-y) / (leftX - x)) <= tanInput) {
			out[0] = x + (botY - y) / tanInput;
			out[1] = botY;
			out[2] = -angle;
		}
		else {
			out[0] = leftX;
			out[1] = y + (leftX - x) * tanInput;
			out[2] = PI - angle;
		}
		break;

	case 4:
		if (((botY - y) / (rightX - x)) >= tanInput) {
			out[0] = x + (botY - y) / tanInput;
			out[1] = botY;
			out[2] = -angle;

		}
		else {
			out[0] = rightX;
			out[1] = y + (rightX - x) * tanInput;
			out[2] = PI - angle;
		}
		break;

	default: break;
	}

	out[3] = fabs((out[0] - x) * tanInput / sinInput);

	return out;
}

void main() {
	srand(time(NULL));

	float barTimeStep = 3117;
	float quarterTimeStep = barTimeStep/4; //milliseconds
	float eigthTimeStep = quarterTimeStep / 2; 
	int timeStart = Time("00:14:690").ms;

	Sprite* bg = new Sprite("sprite/whiteblock.png",Vector2::Zero, Layer::Background);
	bg->Color(Time("00:00:000").ms, Time("10:00:000").ms, Color(0, 0, 0), Color(0, 0, 0));
	bg->Scale(Time("00:00:000").ms, Time("10:00:000").ms, 2000, 2000);
	//StrokeManager* strokeManager = new StrokeManager("Characters");

	//Character test = strokeManager->Get("imsocool");
	
	////Sprite* sprite = new Sprite("image.png", Vector2::Zero, Layer::Foreground, Origin::Centre);
	////sprite->Move(0, Time("00:10:000").ms, Vector2(-100.0f,-100.0f), Vector2(100.0f, 100.0f));
	//std::vector<float> x;
	//std::vector<float> y;

	////Section 1
	 
	////


	////"dot" effect
	std::vector<Sprite*> dots;

	int rcolor;
	int gcolor;
	int bcolor;

	int gcolor2;
	int rcolor2;
	int bcolor2;

	PoissonGenerator::DefaultPRNG randomPoint;

	std::vector<int> startTime = { timeStart, Time("02:44:300").ms };
	std::vector<int> endTime = { Time("01:38:846").ms, Time("03:06:118").ms };
	int currTime = 0;
	int currTime2 = 0;
	float timeNext;
	int n = 25; //number of dots
	float dotScale = 5;
	const auto dotCoord = PoissonGenerator::GeneratePoissonPoints((n+n/2), randomPoint, 10000, false);
	for (int section = 0; section < 2; section++) {
		for (int i = 0; i < n; i++) {
			Sprite *dot = new Sprite("sprite/lildot.png", Vector2(dotCoord[i].x*Vector2::ScreenSize.x - Vector2::ScreenSize.x / 2, dotCoord[i].y*Vector2::ScreenSize.y - Vector2::ScreenSize.y / 2), Layer::Foreground, Origin::Centre);
			currTime = startTime[section];
			currTime2 = startTime[section];
			dot->Scale(startTime[section], startTime[section] + 1, dotScale, dotScale);
			gcolor = rand() % 255;
			rcolor = rand() % (gcolor + 1);
			bcolor = rand() % 10 + 245;

			for (int j = 0; currTime < endTime[section]; j++) {//this is for the first part

				currTime = currTime2;
				currTime2 += quarterTimeStep;

				gcolor2 = rand() % 255;
				rcolor2 = rand() % (gcolor2 + 1);
				bcolor2 = rand() % 10 + 245;

				dot->Color(currTime, currTime2, Color(rcolor, gcolor, bcolor), Color(rcolor2, gcolor2, bcolor2), Easing::Linear);

				dot->Scale(currTime - 200, currTime, dotScale*1, dotScale*1.1, Easing::QuadOut);
				dot->Scale(currTime, currTime + 200, dotScale*1.1, dotScale*1, Easing::QuadOut);

				rcolor = rcolor2;
				gcolor = gcolor2;
				bcolor = bcolor2;
			}

			float velocity = 0.1; //pixel/ms or 1000 pixels/s
			float tau = 2 * PI;
			float angle = fmod(rand(), tau);

			float startX = dotCoord[i].x*Vector2::ScreenSize.x - Vector2::ScreenSize.x / 2;
			float startY = dotCoord[i].y*Vector2::ScreenSize.y - Vector2::ScreenSize.y / 2;

			float time = startTime[section];

			std::vector<float> coord = box_collision(startX, startY, angle);
			timeNext = time + coord[3] / (velocity);

			while (time < endTime[section]) {

				startX = coord[0];
				startY = coord[1];
				time = timeNext;

				coord = box_collision(coord[0], coord[1], coord[2]);
				timeNext = time + coord[3] / velocity;
				dot->Move(time, timeNext, startX, startY, coord[0], coord[1], Easing::Linear);
			}

			dot->Fade(startTime[section] - quarterTimeStep, startTime[section], 0, 0.5, Easing::CubicOut);
			dot->Fade(endTime[section], endTime[section] + quarterTimeStep, 0.5, 0, Easing::SineIn);
			dots.push_back(dot);
		}

	////

	//// Create a file StoryboardInputPath.txt in Tegami\Tegami\Tegami
	//// That's kind of a confusing directory, so to be a bit more clear, make sure the text file is placed on the same level as this Tegami.cpp file
	//// In the first line of the file, put the target storyboard path, e.g. C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\696969 Nekomata Master - Tegami\Nekomata Master - Tegami (niseboi).osb
	//// Make sure it's the absolute path and don't worry about escape characters

//This shit's for writing to the storyboard file
	std::ifstream file("storyboardinputpath.txt");
	std::string path;
	std::getline(file, path);
	Storyboard::Instance()->Write(path);
}