#include "DotWave.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Time.hpp"
#include "Wave.hpp"

#include <sstream>
#include <string>

DotWave::DotWave(Utility *utility, std::vector<Wave> wave, int numberOfDots, float startX, float endX, float lineY) {
	std::vector<Sprite*> waves;
	int a = 0;
	int b = 1;
	int c = 0;

	int i = 0;
	float currTime = wave[0].timeStart;
	float endTime = wave.back().timeEnd;
	float timeElapse;

	std::vector<std::string> commands;

	std::ostringstream lineOne, lineTwo, lineThree;

	std::vector<int> waveCheck;

	float startTime = wave[0].timeStart;

	for (i = 0; i < numberOfDots; i++) {

		Sprite *dot = new Sprite("sprite/wavedot.png", Vector2(startX + (i*(abs(endX - startX) / numberOfDots)), lineY), Layer::Foreground, Origin::Centre);
		dot->Fade(startTime, startTime + utility->quarterTimeStep, 0, 1);
		int j = 0;

		int directionFlag = 1;
		float topPosition = lineY + wave[j].amplitude;
		float bottomPosition = lineY - wave[j].amplitude;
		float initialPosition = bottomPosition;
		float finalPosition = topPosition;
		float initialTime, finalTime;

		float timeDifference = 0;
		float offsetValue;
		std::vector<int> easingFlag{ 17,17 };

		currTime = startTime;

		if (i != 0) {
			b = 0;
		}

		for (j = 0; j < wave.size(); j++) {
			dot->Color(wave[j].timeStart, wave[j].timeEnd, wave[j].color, wave[j].color);
			dot->Scale(wave[j].timeStart, wave[j].timeEnd, wave[j].scale, wave[j].scale);

			topPosition = lineY + wave[j].amplitude;
			bottomPosition = lineY - wave[j].amplitude;


			offsetValue = ((wave[j].wavelength) / wave[j].velocity) / numberOfDots;

			timeElapse = (wave[j].wavelength / wave[j].velocity) / 4;

			while ((currTime < wave[j].timeEnd) && (b == 1)) {
				if (directionFlag == 1) { //down
					initialPosition = finalPosition;
					finalPosition = bottomPosition;
					initialTime = currTime;
					currTime += (2 * timeElapse);
					finalTime = currTime;
					directionFlag = 2;
				}
				else if (directionFlag == 2) { //up
					initialPosition = finalPosition;
					finalPosition = topPosition;
					initialTime = currTime;
					currTime += (2 * timeElapse);
					finalTime = currTime;
					directionFlag = 1;
				}
				Sprite *dot1 = new Sprite("sprite/wavedot.png", Vector2(0, 0), Layer::Foreground, Origin::Centre);
				Color color2(255, 0, 0); //red

				dot1->Color(initialTime, finalTime, color2, color2); //red
				dot->MoveY(initialTime, finalTime, initialPosition, finalPosition, Easing::SineInOut);
				a++;
			} //end of while loop1
			if (b) {
				waveCheck.push_back(a);
			}
			while ((c < waveCheck[j]) && (b == 0)) {
				if (directionFlag == 1) { //down
					initialPosition = finalPosition;
					finalPosition = bottomPosition;
					initialTime = currTime;
					currTime += (2 * timeElapse);
					finalTime = currTime;
					directionFlag = 2;
				}
				else if (directionFlag == 2) { //up
					initialPosition = finalPosition;
					finalPosition = topPosition;
					initialTime = currTime;
					currTime += (2 * timeElapse);
					finalTime = currTime;
					directionFlag = 1;
				}
				Sprite *dot1 = new Sprite("sprite/wavedot.png", Vector2(0, 0), Layer::Foreground, Origin::Centre);
				Color color2(255, 0, 0); //red
				dot1->Color(initialTime, finalTime, color2, color2); //red
				dot->MoveY(initialTime, finalTime, initialPosition, finalPosition, Easing::SineInOut);
				c++;
			} //end of while loop 2
			a = 0;
			c = 0;
		} //end of j loop

		startTime += offsetValue;
		endTime += offsetValue;
		dot->Fade(endTime - utility->quarterTimeStep, endTime, 1, 0);
		waves.push_back(dot);
	} //end of i loop
}