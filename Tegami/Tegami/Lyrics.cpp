#include "Bezier.hpp"
#include "Character.hpp"
#include "Lyrics.hpp"
#include "Sprite.hpp"
#include "Stroke.hpp"
#include "StrokeManager.hpp"
#include "Time.hpp"

Lyrics::Lyrics(std::string fileName){
	StrokeManager strokeManager = StrokeManager("C:\\Users\\Royce\\Documents\\Tegami\\Tegami\\Tegami\\Characters");
	
	std::ifstream file(fileName);
	std::string line;
	std::string siFile;
	std::string startT;
	std::string endT;
	std::string fadeT;
	float xPoint;
	float yPoint;

	for (int i = 0; i < 3; i++) {
		std::string siName(100, 0);
		file >> siFile >> startT >> endT >> fadeT >> timeDifference >> x >> y;
		std::cout << siFile << std::endl;

		startTime = Time(startT).ms;
		endTime = Time(endT).ms;
		fadeTime = Time(fadeT).ms;

		Character singleChar = strokeManager.Get(siFile);
		drawCharacter(singleChar);
		siName.clear();
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
	float dotDensity = 1; //unit is dot/pixel

	Bezier stroke = Bezier(input.points);
	
	float dotbIgBoy = stroke.length * dotDensity; //unit is dot, the number of dots in the line
	float timeStep = 1 / dotbIgBoy; //unit is t, a parametrized unit

	float currTime = 0;

	for (int i = 0; i < dotbIgBoy; i++) {

		Vector2 pos = stroke.findPosition(currTime) + Vector2(x,y);
		Sprite *dot = new Sprite("m.png", pos, Layer::Background, Centre);

		float dotScale = 0.05;
		dot->Scale(fadeIn + timeDifference, fadeIn + 2 * timeDifference, dotScale, dotScale);
		dot->Color(fadeIn + timeDifference, fadeIn + 2 * timeDifference, Color(255, 255, 255), Color(255, 255, 255));
		dot->Fade(fadeIn + timeDifference, fadeIn + 2 * timeDifference, 0, 1);
		dot->Fade(fadeTime, fadeTime + timeDifference, 1, 0);
		
		fadeIn += timeDifference;
		currTime += timeStep;
	}

	return fadeIn;
}