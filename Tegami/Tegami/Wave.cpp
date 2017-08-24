#include "Wave.hpp"



Wave::Wave(float amplitude, float wavelength, float velocity, float timeStart, float timeEnd)

	:amplitude(amplitude), wavelength(wavelength), velocity(velocity/1000),	timeStart(timeStart), timeEnd(timeEnd){
}
