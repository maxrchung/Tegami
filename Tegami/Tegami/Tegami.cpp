#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <fstream>

void main() {
	Sprite* sprite = new Sprite("");
	sprite->Move(0, 0, Vector2::Zero, Vector2::Zero);

	// Create a file StoryboardInputPath.txt in Tegami\Tegami\Tegami
	// That's kind of a confusing directory, so to be a bit more clear, make sure the text file is placed on the same level as this Tegami.cpp file
	// In the first line of the file, put the target storyboard path, e.g. C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\696969 Nekomata Master - Tegami\Nekomata Master - Tegami (niseboi).osb
	// Make sure it's the absolute path and don't worry about escape characters
	std::ifstream file("StoryboardInputPath.txt");
	std::string path;
	std::getline(file, path);
	Storyboard::Instance()->Write(path);
}