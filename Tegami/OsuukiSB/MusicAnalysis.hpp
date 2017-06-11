#ifndef MUSICANALYSIS_HPP
#define MUSICANALYSIS_HPP
#define _USE_MATH_DEFINES

// Have to do this because I can't seem to be able to set an array's length 
// with a variable, even if I specify it as const. This is used to evaluate 
// how many samples you should take at a time for each window, and this is 
// also the size of the FFT arrays
#define WINSIZE 8192

#include "kiss_fftr.h"
#include "MusicAnalysisConfig.hpp"
#include "MusicAnalysisData.hpp"
#include "Wav.hpp"
#include <fstream>
#include <iostream>
#include <vector>

class MusicAnalysis {
public:
	MusicAnalysis(MusicAnalysisConfig config);
	static MusicAnalysisData LoadData(const std::string& dataPath);
private:
	Wav LoadWavFile(const char* fname);
	float CalculateFreqPower(int freqBandStart, int freqBandEnd, int divisions);
	std::vector<int> CalculateFrequencyBands(int freqBandStart, int freqBandEnd, int divisions, const Wav& song);
	float Hann(short in, int index, int size);
	MusicAnalysisData TakeSnapshots(int snapshotRate, const std::vector<int>& freqBandIndices, const Wav& song);
	void SaveData(MusicAnalysisData& data, const std::string& destinationPath);
};

#endif//MUSICANALYSIS_HPP