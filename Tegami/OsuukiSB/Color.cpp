#include "Color.hpp"

Color::Color()
	: r(0), g(0), b(0) {

}

Color::Color(float r, float g, float b) 
	: r(r), g(g), b(b) {
}

Color::Color(float v) 
	: r(v), g(v), b(v) {
}

bool Color::operator==(Color c) {
	return r == c.r && g == c.g && b == c.b;
}

bool Color::operator!=(Color c) {
	return !(*this == c);
}

Color Color::operator*(float f) {
	return Color(r * f, g * f, b * f);
}