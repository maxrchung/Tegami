#include "DotWave.hpp"
#include "Sprite.hpp"
#include "Storyboard.hpp"
#include "Time.hpp"
#include "Wave.hpp"

#include <sstream>
#include <string>

DotWave::DotWave(Utility *utility, std::vector<Wave> wave, int numberOfDots, float startX, float endX, float lineY) {
	std::vector<Sprite*> waves;
	float endTime = wave.back().timeEnd;
	float timeElapse;

	std::vector<int> waveCheck;
	float startTime = wave[0].timeStart;

	for (int i = 0; i < numberOfDots; i++) {
		// numberOfDots - 1 to account for a dot at endX
		Sprite *dot = new Sprite("m.png", Vector2(startX + (i*(abs(endX - startX) / (numberOfDots - 1))), lineY), Layer::Foreground, Origin::Centre);
		dot->Fade(startTime, startTime + utility->quarterTimeStep, 0, 1);

		bool directionFlag = 1;
		float topPosition = lineY + wave[0].amplitude;
		float bottomPosition = lineY - wave[0].amplitude;

		float initialPosition = bottomPosition;
		float finalPosition = topPosition;

		float initialTime, finalTime;

		float offsetValue;

		float currTime = startTime;

		for (int j = 0; j < wave.size(); j++) {

			int a = 0;
			float initialTime, finalTime;

			topPosition = lineY + wave[j].amplitude;
			bottomPosition = lineY - wave[j].amplitude;

			std::vector<float> posVec {topPosition, bottomPosition};

			dot->Color(wave[j].timeStart, wave[j].timeEnd, wave[j].color, wave[j].color);
			dot->Scale(wave[j].timeStart, wave[j].timeEnd, wave[j].scale, wave[j].scale);

			offsetValue = ((wave[j].wavelength) / wave[j].velocity) / numberOfDots;

			timeElapse = (wave[j].wavelength / wave[j].velocity) / 4;

			while (((currTime < wave[j].timeEnd) && (!i)) || ((i) && (a < waveCheck[j]))) { //the order of ((i) && (c < waveCheck[j])) is important, don't change
				
				initialPosition = finalPosition;
				finalPosition = posVec[directionFlag];

				directionFlag = !directionFlag;
				initialTime = currTime;
				currTime += (2 * timeElapse);
				finalTime = currTime;

				dot->MoveY(initialTime, finalTime, initialPosition, finalPosition, Easing::SineInOut);
				a++;
			} //end of while loop1
			if (!i) {
				waveCheck.push_back(a);
			}
		} //end of j loop

		startTime += offsetValue;
		endTime += offsetValue;
		dot->Fade(endTime - utility->quarterTimeStep, endTime, 1, 0);
		waves.push_back(dot);
	} //end of i loop
}