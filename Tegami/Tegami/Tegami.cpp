#include "DotGenerator.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "StrokeAnimation.hpp"
#include "Time.hpp"
#include "Tree.hpp"
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
	bg->Color(Time("00:00:000").ms, Time("10:00:000").ms, Color(0,0,0), Color(0,0,0));
	bg->ScaleVector(Time("00:00:000").ms, Time("10:00:000").ms, Vector2::ScreenSize, Vector2::ScreenSize);

	Tree *tree = new Tree(utility, Vector2::Vector2(0, -100), 0, PI/5, 0, Time("01:00:000").ms, 0.2, 0.8, 8, Color::Color(255,255,255), Color::Color(255,170,180));

	int numberOfDots = 20;
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
		Wave(100, 200, 20, Time("03:46:648").ms, Time("03:55:989").ms, Color(0, 0, 255), 0.5)
	};
	DotWave *waves4 = new DotWave(utility, wave4, numberOfDots, startX, endX, lineY);

	
	int numberOfDots2 = 25;
	float dotVelocity = 0.1;
	float dotScale = 2;
	DotGenerator *dotGenerator1 = new DotGenerator(utility, numberOfDots2, dotScale, dotVelocity, Time("00:14:690").ms, Time("01:38:846").ms);
	DotGenerator *dotGenerator2 = new DotGenerator(utility, numberOfDots2, dotScale, dotVelocity, Time("02:44:300").ms, Time("03:06:118").ms);


	// Create a file StrokeAnimationInputPath.txt in Tegami\Tegami\Tegami
	// Make sure the text file is placed on the same level as this Tegami.cpp file
	// In the first line of the file, put the target SA file path, e.g. C:\Users\Wax Chug da Gwad\Desktop\Tegami\Tegami\Tegami\StrokeAnimation\tegami.sa
	// Make sure it's the absolute path and don't worry about escape characters
	std::ifstream saFile("StrokeAnimationInputPath.txt");
	std::string saPath;
	std::getline(saFile, saPath);
	StrokeAnimation strokeAnimation(utility, saPath);
	
	// Create a file named StoryboardInputPath.txt in Tegami\Tegami\Tegami
	// That's kind of a confusing directory, so to be a bit more clear, make sure the text file is placed on the same level as this Tegami.cpp file
	// In the first line of the file, put the target storyboard path, e.g. C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\696969 Nekomata Master - Tegami\Nekomata Master - Tegami (niseboi).osb
	// Make sure it's the absolute path and don't worry about escape characters
	std::ifstream sbFile("StoryboardInputPath.txt");
	std::string sbPath;
	std::getline(sbFile, sbPath);
	Storyboard::Instance()->Write(sbPath);
}
