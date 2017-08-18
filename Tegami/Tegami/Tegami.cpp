#include "DotGenerator.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "StrokeAnimation.hpp"
#include "Time.hpp"
#include "DotWave.hpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <random>

void main() {
	srand(time(NULL));

	Utility *utility = new Utility();

	Sprite* bg = new Sprite("sprite/whiteblock.png", Vector2::Zero, Layer::Background);
	bg->Color(Time("00:00:000").ms, Time("10:00:000").ms, Color(150), Color(150));
	bg->ScaleVector(Time("00:00:000").ms, Time("10:00:000").ms, Vector2::ScreenSize, Vector2::ScreenSize);
	


	float velocity = 0.01;
	Wave wave = Wave(velocity, Time("00:00:000").ms, Time("01:00:000").ms);
	std::vector<Wave> wave1;
	wave1.push_back(wave);

	float numberOfDots = 10;
	float wavelength = 100;
	float amplitude = 30;
	int startX = 10;
	int endX = 1000;
	int lineY = 100;

	DotWave *waveshit = new DotWave(utility, wave1, numberOfDots, amplitude, wavelength, Time("00:00:000").ms, Time("01:00:000").ms, startX, endX, lineY);

	//int numberOfDots = 25;
	//float dotVelocity = 0.1;
	//float dotScale = 2;
	//DotGenerator *dotGenerator1 = new DotGenerator(utility, numberOfDots, dotScale, dotVelocity, Time("00:14:690").ms, Time("01:38:846").ms);
	//DotGenerator *dotGenerator2 = new DotGenerator(utility, numberOfDots, dotScale, dotVelocity, Time("02:44:300").ms, Time("03:06:118").ms);
	//std::string filePath(R"(C:\Users\Wax Chug da Gwad\Desktop\Tegami\Tegami\Tegami\StrokeAnimation\tegami.sa)");
	//StrokeAnimation strokeAnimation(utility, filePath);
	
	//Violin part or something

	//This shit's for writing to the storyboard file
	std::ifstream file("storyboardinputpath.txt");
	std::string path;
	std::getline(file, path);
	Storyboard::Instance()->Write(path);
}
