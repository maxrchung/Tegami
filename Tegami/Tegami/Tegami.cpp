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
	


	//std::vector<Wave> wave1;

	//float velocity = 50; // pixels/s of the crest
	//Wave wave = Wave(velocity/1000, Time("00:00:000").ms, Time("01:00:000").ms);
	//Wave wave = Wave(0, 0, 0);
	std::vector<Wave> wave;
	std::vector<Wave> wave2;
	std::vector<Wave> wave3;
	std::vector<Wave> wave4;

	float numberOfDots = 20;
	int startX = -50;
	int endX = 300;
	int lineY = 0;

	//float ampl = 100;
	//float wavel = 300;
	//float vel = 40;

	//Wave test = Wave(ampl, wavel, vel, Time("00:00:000").ms, Time("01:00:000").ms);
	//wave.push_back(test);

	//DotWave *waveshit = new DotWave(utility, wave, numberOfDots, Time("00:00:000").ms, Time("01:00:000").ms, startX, endX, lineY);

	float part1Velocity = 30;
	float wavelength1 = 100;
	float amp1 = 100;
	Wave violin_p1_s1 = Wave(amp1, wavelength1, part1Velocity, Time("00:39:625").ms, Time("00:52:093").ms, Color(0,0,255), 0.5);
	wave.push_back(violin_p1_s1);

	DotWave *waves = new DotWave(utility, wave, numberOfDots, startX, endX, lineY);


	float part2Velocity = 30;
	float wavelength2 = 100;
	float amp2 = 100;
	Wave violin_p2_s1 = Wave(amp2, wavelength2, part2Velocity, Time("01:17:028").ms, Time("01:41:963").ms, Color(0, 0, 255), 0.5);
	wave2.push_back(violin_p2_s1);

	part2Velocity = 100;
	wavelength2 = 200;
	amp2 = 300;
	Wave violin_p2_s2 = Wave(amp2, wavelength2, part2Velocity, Time("01:41:963").ms, Time("02:16:248").ms, Color(0, 0, 255), 0.5);
	wave2.push_back(violin_p2_s2);

	DotWave *waves2 = new DotWave(utility, wave2, numberOfDots, startX, endX, lineY);


	float part3Velocity = 30;
	float wavelength3 = 100;
	float amp3 = 100;
	Wave violin_p3_s1 = Wave(amp3, wavelength3, part3Velocity, Time("02:19:365").ms, Time("02:44:300").ms, Color(0, 0, 255), 0.5);
	wave3.push_back(violin_p3_s1);

	DotWave *waves3 = new DotWave(utility, wave3, numberOfDots, startX, endX, lineY);

	float part4Velocity;
	float wavelength4;
	float amp4;

	part4Velocity = 1;
	wavelength4 = 2;
	amp4 = 3;
	Wave violin_p4_s1 = Wave(amp4, wavelength4, part4Velocity, Time("03:09:235").ms, Time("03:43:521").ms, Color(0, 0, 255), 0.5); //3:09:235
	wave4.push_back(violin_p4_s1);

	part4Velocity = 4;
	wavelength4 = 5;
	amp4 = 6;
 	Wave violin_p4_s2 = Wave(amp4, wavelength4, part4Velocity, Time("03:43:521").ms, Time("03:46:648").ms, Color(0, 0, 255), 0.5); //3:43:521
	wave4.push_back(violin_p4_s2);

	part4Velocity = 7;
	wavelength4 = 8;
	amp4 = 9;
	Wave violin_p4_s3 = Wave(amp4, wavelength4, part4Velocity, Time("03:46:648").ms, Time("03:55 : 989").ms, Color(0, 0, 255), 0.5); //3:46:648
	wave4.push_back(violin_p4_s3);
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
