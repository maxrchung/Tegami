#ifndef TIME_HPP
#define TIME_HPP

#include <string>

class Time {
public:
	Time(int value);
	Time();
	Time(std::string format);
	int ms;
	std::string format;
};

#endif//TIME_HPP