#include "Bezier.hpp"
#include "Character.hpp"
#include "Lyrics.hpp"
#include "Sprite.hpp"
#include "Stroke.hpp"
#include "StrokeManager.hpp"
#include "Time.hpp"
#include "Utility.hpp"

#include <sstream>

Lyrics::Lyrics(std::string fileName, Utility *utility){

	u = *utility;
	StrokeManager strokeManager = StrokeManager("Characters");
	
	std::ifstream file(fileName);
	std::string line;
	std::string siFile;
	std::string startT;
	std::string endT;
	std::string fadeT;
	float xPoint;
	float yPoint;

	float temp;

	while (std::getline(file, line)) {
		std::stringstream streamLine(line);

		if (line.empty()){
			continue;
		}

		streamLine >> siFile >> startT >> endT >> fadeT >> timeDifference >> x >> y;
		std::cout << siFile << std::endl;

		startTime = Time(startT).ms;
		endTime = Time(endT).ms;
		fadeTime = Time(fadeT).ms;

		Character singleChar = strokeManager.Get(siFile);
		drawCharacter(singleChar);
	}
}

void Lyrics::drawCharacter(Character input) {
	input.scale(0.1);
	float fadeIn = startTime;
	for (auto& i : input.strokes) {
		fadeIn = drawStroke(i, fadeIn);
	}
}

float Lyrics::drawStroke(Stroke input, float fadeIn) {
	float dotDensity = 2; //unit is dot/pixel

	Bezier stroke = Bezier(input.points);
	
	float dotbIgBoy = stroke.length * dotDensity; //unit is dot, the number of dots in the line
	float timeStep = 1 / dotbIgBoy; //unit is t, a parametrized unit

	float currTime = 0;
	float dotScale = 0.05;
	for (int i = 0; i < dotbIgBoy; i++) {

		Vector2 pos = stroke.findPosition(currTime) + Vector2(x,y);
		Sprite *dot = new Sprite("m.png", pos, Layer::Background, Centre);

		dot->Scale(fadeIn + timeDifference, fadeIn + timeDifference, dotScale, dotScale);
		dot->Color(fadeIn + timeDifference, fadeIn + timeDifference, Color(255, 255, 255), Color(255, 255, 255));

		auto fade = 1 - i / (dotbIgBoy);
		dot->Fade(fadeIn + timeDifference, fadeIn + 2 * timeDifference, 0, fade);
		dot->Fade(fadeTime, fadeTime + u.quarterTimeStep, fade, 0);
		
		fadeIn += timeDifference;
		currTime += timeStep;
	}

	return fadeIn;
}

float Lyrics::getCharacterPositionInLine(float midLinePos, int characterCount, float characterWidth, float characterSpacing, int characterIndex) const {
	if (characterIndex < 0 || characterIndex >= characterCount) {
		throw "Invalid characterIndex";
	}

	// Calculate total distance of line
	float totalCharacterWidth = characterCount * characterWidth;
	float totalCharacterSpacing = (characterCount - 1) * characterSpacing;
	// Gets half value
	float halfTotalDistance = (totalCharacterWidth = totalCharacterSpacing) / 2;

	// Calculate indexed position of line
	float indexedCharacterWidth = (characterIndex + 1) * characterWidth;
	float indexedCharacterSpacing = characterIndex * characterSpacing;
	// Account for center position
	float indexedPosition = indexedCharacterWidth + indexedCharacterSpacing + characterWidth / 2;

	// Subtract the two from above for final position
	float finalPosition = indexedPosition - halfTotalDistance;
	return finalPosition;
}