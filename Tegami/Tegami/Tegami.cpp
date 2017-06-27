#include "Character.hpp"
#include "PartGen.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Stroke.hpp"
#include "StrokeManager.hpp"
#include "Time.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void main() {
	//StrokeManager* strokeManager = new StrokeManager("Characters");

	//Character test = strokeManager->Get("imsocool");
	
	////Sprite* sprite = new Sprite("image.png", Vector2::Zero, Layer::Foreground, Origin::Centre);
	////sprite->Move(0, Time("00:10:000").ms, Vector2(-100.0f,-100.0f), Vector2(100.0f, 100.0f));

	//PartGen shit("00:14:690","00:20:000","sprite/image.png",255,127,19,2,0.5);

	//// Create a file StoryboardInputPath.txt in Tegami\Tegami\Tegami
	//// That's kind of a confusing directory, so to be a bit more clear, make sure the text file is placed on the same level as this Tegami.cpp file
	//// In the first line of the file, put the target storyboard path, e.g. C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\696969 Nekomata Master - Tegami\Nekomata Master - Tegami (niseboi).osb
	//// Make sure it's the absolute path and don't worry about escape characters

//This shit's for writing to the storyboard file
	std::ifstream file("storyboardinputpath.txt");
	std::string path;
	std::getline(file, path);
	Storyboard::Instance()->Write(path);


	std::cin.get();

}