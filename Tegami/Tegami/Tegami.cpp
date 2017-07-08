#include "DotGenerator.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Time.hpp"
#include "tinyxml2.h"

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
	bg->Color(Time("00:00:000").ms, Time("10:00:000").ms, Color(0, 0, 0), Color(0, 0, 0));
	bg->Scale(Time("00:00:000").ms, Time("10:00:000").ms, 2000, 2000);

	int numberOfDots = 25;
	float dotVelocity = 0.1;
	float dotScale = 2;

	DotGenerator *dotGenerator1 = new DotGenerator(utility, numberOfDots, dotScale, dotVelocity, Time("00:14:690").ms, Time("01:38:846").ms);
	DotGenerator *dotGenerator2 = new DotGenerator(utility, numberOfDots, dotScale, dotVelocity, Time("02:44:300").ms, Time("03:06:118").ms);


	tinyxml2::XMLDocument doc;
	doc.LoadFile(R"(C:\Users\Wax Chug da Gwad\Desktop\Tegami\Tegami\Tegami\StrokeAnimation\sample.sa)");
	
	//Violin part or something

	// Create a file StoryboardInputPath.txt in Tegami\Tegami\Tegami
	// That's kind of a confusing directory, so to be a bit more clear, make sure the text file is placed on the same level as this Tegami.cpp file
	// In the first line of the file, put the target storyboard path, e.g. C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\696969 Nekomata Master - Tegami\Nekomata Master - Tegami (niseboi).osb
	// Make sure it's the absolute path and don't worry about escape characters
	//This shit's for writing to the storyboard file
	std::ifstream file("storyboardinputpath.txt");
	std::string path;
	std::getline(file, path);
	Storyboard::Instance()->Write(path);
}
