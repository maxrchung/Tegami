#include "DotGenerator.hpp"
#include "Effects.hpp"
#include "Lyrics.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "StrokeAnimation.hpp"
#include "Time.hpp"
#include "Tree.hpp"
#include "DotWave.hpp"
#include "Okaerinasai.hpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <random>

int effects = Effects::ForegroundDots | Effects::Background;

void processEffect(Utility* utility, int bit) {
	switch (bit) {
		case Effects::Background: {
			// Get rid of SB load
			Sprite* bg = new Sprite("t.png", Vector2::Zero, Layer::Background);

			// Back is used for the majority of the time, it has a #lofifade around the edges
			// Towards the finish of song, back fades to end to help fill in Okaerinsai image gaps
			Sprite* end = new Sprite("s.png", Vector2::Zero, Layer::Background);
			end->ScaleVector(Time("03:59:105").ms, Time("03:59:105").ms, Vector2::ScreenSize, Vector2::ScreenSize);
			end->Color(Time("04:24:041").ms, Time("04:32:000").ms, utility->blueBg, Color(255));
			Sprite* back = new Sprite("f.png", Vector2::Zero, Layer::Background);
			auto scale = 480 / 1080.0f;
			back->Scale(Time("00:00:000").ms, Time("10:00:000").ms, scale, scale, Easing::Linear);
			back->Color(Time("00:00:000").ms, Time("00:14:690").ms, Color(), utility->pinkBg);
			back->Color(Time("02:19:365").ms, Time("02:44:300").ms, back->color, utility->blueBg);
			back->Fade(Time("03:59:105").ms, Time("04:05:339").ms, 1, 0);

			//Okaerinasai ok(utility);
			break;
		}
		case Effects::BackgroundDots: {
			// Pink sections
			// A bit more colorful than the blue section below
			DotGenerator(utility, 8,	2.8,	0,		0.01,	0,		utility->pinkBg,	35, 0.5,	0,		Time("00:14:690").ms, Time("01:17:028").ms, "l.png",  false);
			DotGenerator(utility, 30,	0.8,	0.7,	0.005,	0.004,	utility->pinkBg,	25, 0.5,	0.25,	Time("00:27:157").ms, Time("01:29:495").ms, "m.png",  false);
			DotGenerator(utility, 100,	0.05,	0.04,	0.015,	0.01,	Color(255),			20, 0.5,	0.4,	Time("00:39:625").ms, Time("01:40:404").ms, "m.png",  false);

			// Blue sections
			// Blue section is directly copied from above, this has a stronger blue color
			DotGenerator(utility, 8,	2.8,	0,		0.01,	0,		utility->blueBg,	35, 0.5,	0,		Time("02:19:365").ms, Time("03:06:118").ms, "l.png",  false);
			DotGenerator(utility, 30,	0.8,	0.7,	0.005,	0.004,	utility->blueBg,	25, 0.5,	0.25,	Time("02:31:054").ms, Time("03:06:898").ms, "m.png",  false);
			DotGenerator(utility, 100,	0.05,	0.04,	0.015,	0.01,	Color(255),			20, 0.5,	0.4,	Time("02:38:067").ms, Time("03:07:677").ms, "m.png",  false);	

			std::vector<Wave> waves{
				Wave(150, (853 - 50)/4, (853-50/4) * 1000 / (utility->quarterTimeStep * 4 * 2) , Time("01:41:625").ms, Time("02:14:690").ms, Color(255,209,220), 0.5)
			};
			// +25 and -25 are for buffer space
			DotWave(utility, waves, 20, -853.0f/2 + 25, 852.0f/2 -25, 0);

			// 4 from center to outwards
			Tree(utility, Vector2(0, 0),	0,			PI / 4, Time("03:09:235").ms, Time("03:20:144").ms, Time("03:20:144").ms, 8,	0.2,	0.92,	3, 5, Color(255), utility->blueBg);
			Tree(utility, Vector2(0, 0),	PI,			PI / 4, Time("03:09:235").ms, Time("03:20:144").ms, Time("03:20:144").ms, 8,	0.2,	0.92,	3, 5, Color(255), utility->blueBg);
			Tree(utility, Vector2(0, 0),	PI / 2,		PI / 4, Time("03:09:235").ms, Time("03:20:144").ms, Time("03:20:144").ms, 8,	0.2,	0.92,	3, 5, Color(255), utility->blueBg);
			Tree(utility, Vector2(0, 0),	3 * PI / 2,	PI / 4, Time("03:09:235").ms, Time("03:20:144").ms, Time("03:20:144").ms, 8,	0.2,	0.92,	3, 5, Color(255), utility->blueBg);

			// 3 from center to outwards
			Tree(utility, Vector2(0, 0),	PI,			PI / 3, Time("03:21:703").ms, Time("03:32:612").ms, Time("03:32:612").ms, 11,	0.3,	0.7,	3, 7, utility->blueBg, Color(0));
			Tree(utility, Vector2(0, 0),	5 * PI / 3,	PI / 3,	Time("03:21:703").ms, Time("03:32:612").ms, Time("03:32:612").ms, 11,	0.3,	0.7,	3, 7, utility->blueBg, Color(0));
			Tree(utility, Vector2(0, 0),	PI / 3,		PI / 3, Time("03:21:703").ms, Time("03:32:612").ms, Time("03:32:612").ms, 11,	0.3,	0.7,	3, 7, utility->blueBg, Color(0));

			auto rotation = Vector2(1, 0).AngleBetween(Vector2(853 / 2, -240));
			// 4 from corners to center
			Tree(utility, Vector2(-853/2, -240),	PI / 2 - rotation,		rotation, Time("03:34:170").ms, Time("03:45:080").ms, Time("03:45:080").ms, 13,	0.15, 0.8, 2.5, 8, Color(0), Color(51));
			Tree(utility, Vector2(-853/2, 240),		PI / 2 + rotation,		rotation, Time("03:34:170").ms, Time("03:45:080").ms, Time("03:45:080").ms, 13,	0.15, 0.8, 2.5, 8, Color(0), Color(102));
			Tree(utility, Vector2(853/2, -240),		3 * PI / 2 + rotation,	rotation, Time("03:34:170").ms, Time("03:45:080").ms, Time("03:45:080").ms, 13,	0.15, 0.8, 2.5, 8, Color(0), Color(153));
			Tree(utility, Vector2(853/2, 240),		3 * PI / 2 - rotation,	rotation, Time("03:34:170").ms, Time("03:45:080").ms, Time("03:45:080").ms, 13,	0.15, 0.8, 2.5, 8, Color(0), Color(204));

			// "The Quintessential Tree." (2017)
			Tree(utility, Vector2(0, -200), 0, PI / 3, Time("03:46:703").ms, Time("03:55:989").ms, Time("03:55:989").ms, 15, 0.33, 0.65, 2, 8, Color(255), Color(255));

			break;
		}
		case Effects::Animation: {
			StrokeAnimation(utility, "StrokeAnimation/tegami.sa", Time("03:06:118"), Time("04:05:339"));
			break;
		}
		case Effects::ForegroundDots: {
			std::vector<Wave> waves{
				//Wave(150, (853 - 50) / 4, (853 - 50 / 4) * 1000 / (utility->quarterTimeStep * 4 * 2) , Time("01:41:625").ms, Time("02:14:690").ms, Color(255,209,220), 0.5)
				Wave(150, 450, 100 , Time("01:41:625").ms, Time("02:14:690").ms, Color(255,209,220), 0.5)
			};
			// +25 and -25 are for buffer space
			DotWave(utility, waves, 50, -853.0f / 2 + 25, 852.0f / 2 - 25, 0);
			break;
		}
		case Effects::Foreground: {
			Lyrics::Lyrics("Characters\\lyrics.ly");
			break;
		}
	}

	Storyboard::Instance()->Write(std::to_string(bit) + ".osb");
	Storyboard::Instance()->Clear();
}

void main() {
	srand(time(NULL));
	Utility *utility = new Utility();

	for (int bit = 1; bit < Effects::bIgBoy; bit *= 2) {
		if (effects & bit) {
			processEffect(utility, bit);

		}
	}

	// Create a file named StoryboardInputPath.txt in Tegami\Tegami\Tegami
	// That's kind of a confusing directory, so to be a bit more clear, make sure the text file is placed on the same level as this Tegami.cpp file
	// In the first line of the file, put the target storyboard path, e.g. C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\696969 Nekomata Master - Tegami\Nekomata Master - Tegami (niseboi).osb
	// Make sure it's the absolute path and don't worry about escape characters
	std::ifstream sbFile("StoryboardInputPath.txt");
	std::string sbPath;
	std::getline(sbFile, sbPath);

	std::ofstream mainFile;
	mainFile.open(sbPath);
	mainFile << "[Events]" << std::endl;

	for (int bit = 1; bit < Effects::bIgBoy; bit *= 2) {
		std::ifstream partFile;
		std::string fileName(std::to_string(bit) + ".osb");
		partFile.open(fileName);

		std::cout << "Copying: " << fileName << std::endl;

		std::string line;
		while (std::getline(partFile, line)) {
			if ((line[0] == '/' && line[1] == '/') || line == "[Events]") {
				continue;
			}
			mainFile << line << std::endl;
		}
		partFile.close();
	}
	std::cout << "Files combined to: " << sbPath << std::endl;
	mainFile << std::endl; 
	mainFile.close();

	//std::cin.get();
}