#include "DotWave.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Time.hpp"
#include "Wave.hpp"

#include <sstream>
#include <string>

DotWave::DotWave(Utility *utility, std::vector<Wave> wave, int numberOfDots, float startX, float endX, float lineY) {
	std::vector<Sprite*> waves;
	int i = 0;
	float currTime = wave[0].timeStart;
	float endTime = wave.back().timeEnd;
	float timeElapse;

	std::vector<int> waveCheck;

	float startTime = wave[0].timeStart;

	for (i = 0; i < numberOfDots; i++) {

		Sprite *dot = new Sprite("sprite/wavedot.png", Vector2(startX + (i*(abs(endX - startX) / numberOfDots)), lineY), Layer::Foreground, Origin::Centre);
		dot->Fade(startTime, startTime + utility->quarterTimeStep, 0, 1);

		bool directionFlag = 1;
		float topPosition = lineY + wave[0].amplitude;
		float bottomPosition = lineY - wave[0].amplitude;

		float initialPosition = bottomPosition;
		float finalPosition = topPosition;

		float initialTime, finalTime;

		float offsetValue;

		currTime = startTime;

		for (int j = 0; j < wave.size(); j++) {
			int a = 0;

			topPosition = lineY + wave[j].amplitude;
			bottomPosition = lineY - wave[j].amplitude;

			dot->Color(wave[j].timeStart, wave[j].timeEnd, wave[j].color, wave[j].color);
			dot->Scale(wave[j].timeStart, wave[j].timeEnd, wave[j].scale, wave[j].scale);

			offsetValue = ((wave[j].wavelength) / wave[j].velocity) / numberOfDots;

			timeElapse = (wave[j].wavelength / wave[j].velocity) / 4;

			while (((currTime < wave[j].timeEnd) && (i == 0)) || ((i != 0) && (a < waveCheck[j]))) { //the order of ((i != 0) && (c < waveCheck[j])) is important, don't change
				initialPosition = finalPosition;

				if (directionFlag == 1) { //down
					finalPosition = bottomPosition;
					directionFlag = 0;
				}
				else if (directionFlag == 0) { //up
					finalPosition = topPosition;
					directionFlag = 1;
				}

				initialTime = currTime;
				currTime += (2 * timeElapse);
				finalTime = currTime;
				dot->MoveY(initialTime, finalTime, initialPosition, finalPosition, Easing::SineInOut);
				a++;
			} //end of while loop1
			if (i == 0) {
				waveCheck.push_back(a);
			}
		} //end of j loop

		startTime += offsetValue;
		endTime += offsetValue;
		dot->Fade(endTime - utility->quarterTimeStep, endTime, 1, 0);
		waves.push_back(dot);
	} //end of i loop
}