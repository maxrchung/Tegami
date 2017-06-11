#ifndef MUSICANALYSISCONFIG_HPP
#define MUSICANALYSISCONFIG_HPP

#include <string>

// I could've jammed  all of these parameters into MusicAnalysis's constructor,
// but that seemed too messy, so I made a separate config class for this
// loading. Maybe in the future you can load this from a separate text file 
// or something
struct MusicAnalysisConfig {
	// Path to input music file
	std::string musicPath;
	// Path to output data file
	std::string dataPath;

	// What frequency bands to start and end on
	int freqBandStart;
	int freqBandEnd;
	// Number of band separations we want for the spectrum, i.e. number of bars
	int divisions;

	// How fast do we want to read samples in ms, e.g. 1000 snapshotRate means we
	// read samples every second
	int snapshotRate;
};

#endif//MUSICANALYSISCONFIG_HPP