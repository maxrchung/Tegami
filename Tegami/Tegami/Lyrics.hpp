#pragma once
#include "Character.hpp"
#include "Stroke.hpp"
#include "Utility.hpp"

class Lyrics
{
public:
	Lyrics(std::string fileName, Utility *utility);

	int startTime;
	int endTime;
	int fadeTime;
	float timeDifference;
	int x;
	int y;

	void drawCharacter(Character input);
private:
	Utility u;
	float drawStroke(Stroke input, float fadeIn);
};