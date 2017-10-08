#include "Time.hpp"
#include <regex>

Time::Time(int ms) {
	this->ms = ms;
	int minutes = ms / 60000;
	int minusMinutes = ms - minutes * 60000;
	int seconds = minusMinutes / 1000;
	int minusSeconds = minusMinutes - seconds * 1000;
	char buffer[50];
	sprintf(buffer, "%02d:%02d:%03d", minutes, seconds, minusSeconds);
	this->format = buffer;
}

Time::Time() {
	*this = Time(0);
}

Time::Time(std::string format) {
	this->format = format;
	std::smatch smatch;
	std::regex regex(R"(\d{2,})");
	std::regex_search(format, smatch, regex);
	int minutes = std::stoi(smatch[0]);

	format = smatch.suffix().str();
	std::regex_search(format, smatch, regex);
	int seconds = std::stoi(smatch[0]);

	format = smatch.suffix().str();
	std::regex_search(format, smatch, regex);
	int ms = std::stoi(smatch[0]);
	this->ms = minutes * 60000 + seconds * 1000 + ms;
}