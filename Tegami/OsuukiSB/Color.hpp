#ifndef COLOR_HPP
#define COLOR_HPP

#include <vector>

class Color {
public:
	Color();
	Color(float v);
	Color(float r, float g, float b);
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	bool operator==(Color c);
	bool operator!=(Color c);
	Color operator/(float f);
	Color operator*(float f);
	Color operator+(Color a);
	Color operator-(Color a);
};

#endif//COLOR_HPP