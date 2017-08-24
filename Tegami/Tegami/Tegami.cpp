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
	bg->Color(Time("00:00:000").ms, Time("10:00:000").ms, Color(0), Color(0));
	bg->ScaleVector(Time("00:00:000").ms, Time("10:00:000").ms, Vector2::ScreenSize, Vector2::ScreenSize);


	float numberOfDots = 20;
	int startX = -50;
	int endX = 300;
	int lineY = 0;

	std::vector<Wave> wave{ 
		Wave(200, 100, 400, Time("00:39:625").ms, Time("00:52:093").ms, Color(0, 0, 255), 0.5) 
	};
	DotWave *waves = new DotWave(utility, wave, numberOfDots, startX, endX, lineY);


	std::vector<Wave> wave2{
		Wave(100, 200, 100, Time("01:17:028").ms, Time("01:41:963").ms, Color(0, 0, 255), 0.5),
		Wave(300, 200, 300, Time("01:41:963").ms, Time("02:16:248").ms, Color(0, 0, 255), 0.5)
	};
	DotWave *waves2 = new DotWave(utility, wave2, numberOfDots, startX, endX, lineY);


	std::vector<Wave> wave3{
		Wave(100, 200, 20, Time("02:19:365").ms, Time("02:44:300").ms, Color(0, 0, 255), 0.5)
	};
	DotWave *waves3 = new DotWave(utility, wave3, numberOfDots, startX, endX, lineY);


	std::vector<Wave> wave4{
		Wave(100, 200, 20, Time("03:09:235").ms, Time("03:43:521").ms, Color(0, 0, 255), 0.5),
		Wave(100, 200, 20, Time("03:43:521").ms, Time("03:46:648").ms, Color(0, 0, 255), 0.5),
		Wave(100, 200, 20, Time("03:46:648").ms, Time("03:55 : 989").ms, Color(0, 0, 255), 0.5)
	};
	//3:55:989 

	DotWave *waves4 = new DotWave(utility, wave4, numberOfDots, startX, endX, lineY);

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
