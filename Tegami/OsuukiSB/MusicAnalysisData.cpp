#include "MusicAnalysisData.hpp"
#include <iostream>

int MusicAnalysisData::GetMeasureIndex(Time time) {
	float timeDivision = (float) time.ms / snapshotRate;
	// Subtract 1 because the first value in ScaleData starts at time = snapshotRate, not 0
	int closestDivision = (int) roundf(timeDivision) - 1;
	return closestDivision;
}