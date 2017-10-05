#include "DotGenerator.hpp"
#include "Effects.hpp"
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

int effects = Effects::Background;

void processEffect(Utility* utility, int bit) {
	switch (bit) {
		case Effects::Background: {
			Sprite* bg = new Sprite("sprite/solidblock.png", Vector2::Zero, Layer::Background);
			bg->ScaleVector(Time("00:00:000").ms, Time("10:00:000").ms, Vector2::ScreenSize, Vector2::ScreenSize);

			//bg->Color(Time("00:00:000").ms, Time("10:00:000").ms, Color(255, 209, 220), Color(255, 209, 220));
			bg->Color(Time("00:00:000").ms, Time("04:24:041").ms, Color(255, 180, 198), Color(250, 180, 198));
			bg->Color(Time("04:24:041").ms, Time("04:29:885").ms, bg->color, Color(255));

			Okaerinasai ok(utility);
			break;
		}
		case Effects::BackgroundDots: {
			DotGenerator *dotGenerator1 = new DotGenerator(utility, 25, 2, 0.1, Time("00:14:690").ms, Time("01:38:846").ms);
			DotGenerator *dotGenerator2 = new DotGenerator(utility, 25, 2, 0.1, Time("02:44:300").ms, Time("03:06:118").ms);

			std::vector<Wave> wave{
				Wave(200, 100, 400, Time("00:39:625").ms, Time("00:52:093").ms, Color(0, 0, 255), 0.5)
			};
			DotWave *waves = new DotWave(utility, wave, 20, -50, 300, 0);

			std::vector<Wave> wave2{
				Wave(100, 200, 100, Time("01:17:028").ms, Time("01:41:963").ms, Color(0, 0, 255), 0.5),
				Wave(300, 200, 300, Time("01:41:963").ms, Time("02:16:248").ms, Color(0, 0, 255), 0.5)
			};
			DotWave *waves2 = new DotWave(utility, wave2, 20, -50, 300, 0);

			std::vector<Wave> wave3{
				Wave(100, 200, 20, Time("02:19:365").ms, Time("02:44:300").ms, Color(0, 0, 255), 0.5)
			};
			DotWave *waves3 = new DotWave(utility, wave3, 20, -50, 300, 0);

			std::vector<Wave> wave4{
				Wave(100, 200, 20, Time("03:09:235").ms, Time("03:43:521").ms, Color(0, 0, 255), 0.5),
				Wave(100, 200, 20, Time("03:43:521").ms, Time("03:46:648").ms, Color(0, 0, 255), 0.5),
				Wave(100, 200, 20, Time("03:46:648").ms, Time("03:55:989").ms, Color(0, 0, 255), 0.5)
			};
			DotWave *waves4 = new DotWave(utility, wave4, 20, -50, 300, 0);

			Tree *tree = new Tree(utility, Vector2(0, -100), 0, PI / 5, 0, Time("01:00:000").ms, 0.2, 0.8, 8, Color(255, 255, 255), Color(255, 170, 180));
			break;
		}
		case Effects::Animation: {
			StrokeAnimation strokeAnimation(utility, "StrokeAnimation/tegami.sa", Time("03:50:00"));
			break;
		}
		case Effects::ForegroundDots: {
			break;
		}
		case Effects::Foreground: {
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

	std::cin.get();
}