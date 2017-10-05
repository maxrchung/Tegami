#pragma once
#include "Character.hpp"
#include "Stroke.hpp"

class Lyrics
{
public:
	Lyrics(std::string fileName);

	float startTime;
	float endTime;
	float timeDifference;

	void drawCharacter(Character input);
private:
	float drawStroke(Stroke input, float fadeIn);
};