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

	// Gets the center position for a character of a centered line
	// Note that this should work for both horizontal and vertical lines if you use this correctly
	// midLinePos:			Where the center of the line should be
	// characterCount:		How many characters there are in a line
	// characterWidth:		How wide a character is; all characters will be based off this width
	// characterSpacing:	Extra padding between two characters
	// characterIndex:		The index of the character you would like to find the position for; starts at 0
	float getCharacterPositionInLine(float midLinePos, int characterCount, float characterWidth, float characterSpacing, int characterIndex) const;

private:
	Utility u;
	float drawStroke(Stroke input, float fadeIn);
};