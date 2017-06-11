#ifndef MUSICANALYSISDATA_HPP
#define MUSICANALYSISDATA_HPP

#include "Time.hpp"
#include <vector>

// This class is used to output music analyzed info in and out of files
// The reason why I used files is because the process of analyzing music
// is not a trivial one. So to save time, we save the data into a particular format 
// and read it in later when we need it.

// Most import in this class is the <std::vector<std::vector<float>> structure
// This corresponds to a float list of normalized scale values for each spectrum bar

typedef std::vector<std::vector<float>> ScaleData;

class MusicAnalysisData {
public:
	int snapshotRate;
	ScaleData scaleData;
	int bandCount;
	// Number of measurements
	int scaleCount;
	int songLength;

	// Finds the closest measuring to the specified time
	int GetMeasureIndex(Time time);
};

#endif//MUSICANALYSISDATA_HPP