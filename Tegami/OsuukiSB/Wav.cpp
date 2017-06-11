#include "Wav.hpp"
#include <sstream>

std::ostream& operator<<(std::ostream& os, const Wav& wav) {
	os << "chunkId: " << wav.chunkId << std::endl;
	os << "chunkSize: " << wav.chunkSize << std::endl;
	os << "format: " << wav.format << std::endl;
	os << "subchunk1Id: " << wav.subchunk1Id << std::endl;
	os << "subchunk1Size: " << wav.subchunk1Size << std::endl;
	os << "audioFormat: " << wav.audioFormat << std::endl;
	os << "numChannels: " << wav.numChannels << std::endl;
	os << "sampleRate: " << wav.sampleRate << std::endl;
	os << "byteRate: " << wav.byteRate << std::endl;
	os << "blockAlign: " << wav.blockAlign << std::endl;
	os << "bitsPerSample: " << wav.bitsPerSample << std::endl;
	os << "subchunk2Id: " << wav.subchunk2Id << std::endl;
	os << "subchunk2Size: " << wav.subchunk2Size << std::endl;
	os << "size: " << wav.size << std::endl;

	return os;
}