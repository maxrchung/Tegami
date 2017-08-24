#pragma once

#include <vector>

#include "Utility.hpp"
#include "Wave.hpp"

//velocity is the speed of the crest of the sine wave in pixels/ms or 1000pixels/s
//wavelength and amplitude are in units of pixels
class DotWave{
public:
	DotWave(Utility *utility, std::vector<Wave> wave, int numberOfDots, float startX, float endX, float lineY);

private:

};

