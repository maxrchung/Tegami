#include "Color.hpp"
#include "Wave.hpp"

Wave::Wave(float amplitude, float wavelength, float velocity, float timeStart, float timeEnd, Color color, float scale)
	:amplitude(amplitude), wavelength(wavelength), velocity(velocity),	timeStart(timeStart), timeEnd(timeEnd), color(color), scale(scale){
}
