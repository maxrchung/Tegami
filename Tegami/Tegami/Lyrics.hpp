#pragma once
#include "Character.hpp"
#include "Stroke.hpp"

class Lyrics
{
public:
	Lyrics(std::string fileName);

	int startTime;
	int endTime;
	int fadeTime;
	float timeDifference;
	int x;
	int y;

	void drawCharacter(Character input);
private:
	float drawStroke(Stroke input, float fadeIn);
};