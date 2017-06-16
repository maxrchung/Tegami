#include "MusicAnalysis.hpp"

MusicAnalysis::MusicAnalysis(MusicAnalysisConfig config) {
	std::string musicPath = config.musicPath;
	Wav song = LoadWavFile(musicPath.c_str());

	// Actual indices corresponding to the WINSIZE
	std::vector<int> freqBandIndices = CalculateFrequencyBands(config.freqBandStart, 
		config.freqBandEnd, 
		config.divisions, 
		song);

	// Take spectrum snapshots and store in data
	MusicAnalysisData data = TakeSnapshots(config.snapshotRate, freqBandIndices, song);

	SaveData(data, config.dataPath);
}

// Grabs info from a WAV file and puts it into a class
// Uses C-style because of the guide I referenced off of: http://rogerchansdigitalworld.blogspot.com/2010/05/how-to-read-wav-format-file-in-c.html
// WAV format: http://soundfile.sapp.org/doc/WaveFormat/wav-sound-format.gif
Wav MusicAnalysis::LoadWavFile(const char* fname) {
	FILE* fp = fopen(fname, "rb");
	Wav wav;

	fread(wav.chunkId, sizeof(char), 4, fp);
	// Without the '\0', printing to console will try and print out later characters
	wav.chunkId[4] = '\0';
	fread(&wav.chunkSize, sizeof(unsigned long), 1, fp);
	fread(wav.format, sizeof(char), 4, fp);
	wav.format[4] = '\0';

	fread(wav.subchunk1Id, sizeof(char), 4, fp);
	wav.subchunk1Id[4] = '\0';
	fread(&wav.subchunk1Size, sizeof(unsigned long), 1, fp);
	fread(&wav.audioFormat, sizeof(short), 1, fp);
	fread(&wav.numChannels, sizeof(short), 1, fp);
	fread(&wav.sampleRate, sizeof(unsigned long), 1, fp);
	fread(&wav.byteRate, sizeof(unsigned long), 1, fp);
	fread(&wav.blockAlign, sizeof(short), 1, fp);
	fread(&wav.bitsPerSample, sizeof(short), 1, fp);

	fread(wav.subchunk2Id, sizeof(char), 4, fp);
	wav.subchunk2Id[4] = '\0';
	// subchunk2Size returns the amount of WAV data in bytes
	fread(&wav.subchunk2Size, sizeof(unsigned long), 1, fp);
	// size is the actual number of shorts
	wav.size = wav.subchunk2Size / sizeof(short);
	wav.data = (short*)malloc(wav.subchunk2Size);
	fread(wav.data, sizeof(short), wav.size, fp);

	fclose(fp);
	return wav;
}

// The calculation behind finding freqPower; required some natural log math
// s * f^d = e where
// s = freqBandStart
// e = freqBandEnd
// d = divisions
// f = factor to find
// ln(s * f^d) = ln(e)
// ln(s) + ln(f^d) = ln(e)
// ln(s) + d * ln(f) = ln(e)
// d * ln(f) = ln(e) - ln(s)
// ln(f) = (ln(e) - ln(s)) / d
// f = e ^ ((ln(e) - ln(s)) / d)
float MusicAnalysis::CalculateFreqPower(int freqBandStart, int freqBandEnd, int divisions) {
	// Splitting up so I can visualize better
	float freqPower = logf(freqBandEnd);
	freqPower = freqPower - logf(freqBandStart);
	freqPower = freqPower / divisions;
	freqPower = powf(expf(1), freqPower);
	return freqPower;
}

std::vector<int> MusicAnalysis::CalculateFrequencyBands(int freqBandStart, int freqBandEnd, int divisions, const Wav& song) {
	float freqPower = CalculateFreqPower(freqBandStart, freqBandEnd, divisions);
	// Size is division + 1 mostly because of later steps. The freqBandIndices 
	// represent ranges that a frequency falls under. For example, for the first bar,
	// freqBandIndices[0] represents the start FFT bin index to look through and
	// freqBandIndices[1] represents the end FFT bin index to look through. The reason
	// the size is divisions + 1 is because the last bar needs an extra index for its
	// end bin.
	// One alternative that I might end up doing is making some kind of Range class to
	// make this kind of exception less of a hassle to remember, but I understand it as of now,
	// so I'll defer changes down the line
	std::vector<int> freqBandIndices(divisions + 1);

	// Find the corresponding indices reflecting the freqs after FFT.
	// FFT divides up the output into linear bins of frequencies. Each
	// value inside freqBandIndices represents the starting bin you should
	// look in. If you are not the last freqBand, see the next index to
	// determine the end range of bins you should search in.
	// The nth FFT bin is in n * sampleRate / WINSIZE frequency
	float freqConstant = (float)song.sampleRate / WINSIZE;
	float freq = freqBandStart;
	for (int i = 0; i < freqBandIndices.size(); ++i) {
		int freqBandIndex = freq / freqConstant;
		freqBandIndices[i] = freqBandIndex;
		freq *= freqPower;
	}
	return freqBandIndices;
}

// Hann function used for TakeSnapshots
float MusicAnalysis::Hann(short in, int index, int size) {
	// Doing this split up shenanigans to show steps
	float value = 2.0f * M_PI * index;
	value = value / (size - 1.0f);
	value = cos(value);
	value = 1 - value;
	value = value * 0.5f;
	value = value * in;
	return value;
}

// Does the big bad job of running through samples,
// computing FFT and scaling bars
MusicAnalysisData MusicAnalysis::TakeSnapshots(int snapshotRate, const std::vector<int>& freqBandIndices, const Wav& song) {
	MusicAnalysisData data;
	data.snapshotRate = snapshotRate;
	data.scaleData = ScaleData(freqBandIndices.size() - 1);

	// We do snapshotRate, this assumes mono channel. I ran into some
	// problems with trying to do dual channel because I think the empty
	// sections screw over some parts of the FFT. 
	float snapshotsPerSec = 1000.0f / snapshotRate;
	// A little on the distinction between snapshots and progress:
	// Snapshot rate describes how often we're taking snapshots, and
	// progress rate describes how many respective samples we need to
	// progress through to match the snapshot rate
	// Note also that sample rate is given in Hz, samples per second
	float progressRate = song.sampleRate / snapshotsPerSec;

	// Debug info
	//std::cout << snapshotsPerSec << std::endl;
	//std::cout << progressRate << std::endl;
	//std::cout << song << std::endl;
	
	// Find start and end points we take samples from
	// Need to account for window sizes, so we start and end with a bit of buffer space
	for (float p = progressRate; p < song.size - progressRate; p += progressRate) {
		// Displays progress at so and so seconds
		std::cout << "Processing sample at: " << (int) (p / song.sampleRate) << " seconds" << std::endl;

		// Grab WINSIZE amount of samples and apply Hann function to them
		float* input = (float*)malloc(WINSIZE * sizeof(float));
		for (int j = 0; j < WINSIZE; ++j) {
			// Taking data half below and after makes sure that our window
			// is centered on where we want to take our snapshot
			short data = song.data[j + (int)p - (WINSIZE / 2)];
			float hann = Hann(data, j, WINSIZE);
			// Make sure to divide!! FFT takes input from -1 to 1 but WAV
			// gives short values between -32000~ to 32000~
			input[j] = hann / 32768.0f;
		}

		// Setup FFT and apply FFT to input
		// Since we are using real values only, I use the real version of kiss_fft
		kiss_fft_cpx out[WINSIZE / 2 + 1];
		kiss_fftr_cfg cfg = kiss_fftr_alloc(WINSIZE, 0, NULL, NULL);
		kiss_fftr(cfg, input, out);
		free(cfg);

		// Condense FFT output into frequency band information
		for (int j = 0; j < freqBandIndices.size() - 1; ++j) {
			// Find bin indices
			int startBin = freqBandIndices[j];
			int endBin = freqBandIndices[j + 1];

			// Track the max value out of the respective bins
			// You don't have to use max, but a resource I referenced used it: https://github.com/zardoru/osutk/blob/master/tools/musnalisys.py
			// Special thanks to Mr. Zardoru btw, helped point me in the right direction to fix some frequency issues
			float maxMagSquared = 0.0f;
			for (int k = startBin; k < endBin; ++k) {
				float magSquared = out[k].r * out[k].r + out[k].i * out[k].i;
				if (magSquared > maxMagSquared) {
					maxMagSquared = magSquared;
				}
			}
			
			// Scale the magnitude to a more reasonable log/dB scale
			float loggedMax = 10.0f * log10f(maxMagSquared);
			// log10 can return from negative infinity to 0, so this clamps negative values
			if (loggedMax < 0.0f) {
				loggedMax = 0.0f;
			}

			data.scaleData[j].push_back(loggedMax);
		}
	}

	return data;
}

void MusicAnalysis::SaveData(MusicAnalysisData& data, const std::string& destinationPath) {
	std::ofstream output(destinationPath);
	output << data.snapshotRate << std::endl;
	
	// For easier time loading back in, prints number of bars
	data.bandCount = data.scaleData.size();
	output << data.bandCount << std::endl;

	// And prints number of values per bar
	data.scaleCount = data.scaleData[0].size();
	output << data.scaleCount << std::endl;

	data.songLength = data.scaleCount * data.snapshotRate;
	output << data.songLength << std::endl;

	for (int i = 0; i < data.bandCount; ++i) {
		for (int j = 0; j < data.scaleCount; ++j) {
			output << data.scaleData[i][j] << std::endl;
		}
	}

	output.close();
}

MusicAnalysisData MusicAnalysis::LoadData(const std::string& dataPath) {
	std::ifstream input(dataPath);

	int snapshotRate;
	input >> snapshotRate;

	int bandCount;
	input >> bandCount;

	int sampleCount;
	input >> sampleCount;

	int songLength;
	input >> songLength;

	ScaleData scaleData;
	for (int i = 0; i < bandCount; ++i) {
		std::vector<float> barData;
		for (int j = 0; j < sampleCount; ++j) {
			float scaleValue;
			input >> scaleValue;
			barData.push_back(scaleValue);
		}
		scaleData.push_back(barData);
	}

	input.close();

	MusicAnalysisData data;
	data.snapshotRate = snapshotRate;
	data.scaleData = scaleData;
	data.bandCount = bandCount;
	data.scaleCount = sampleCount;
	data.songLength = songLength;

	return data;
}