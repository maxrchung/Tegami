#ifndef EASING_HPP
#define EASING_HPP

#include <string>

enum Easing {
	Linear,
	EasingOut, // Same as QuadOut - cl8n
	EasingIn, // Same as QuadIn - cl8n
	QuadIn,
	QuadOut,
	QuadInOut,
	CubicIn,
	CubicOut,
	CubicInOut,
	QuartIn,
	QuartOut, // 10
	QuartInOut,
	QuintIn,
	QuintOut,
	QuintInOut,
	SineIn,
	SineOut,
	SineInOut,
	ExpoIn,
	ExpoOut,
	ExpoInOut, // 20
	CircIn,
	CircOut,
	CircInOut,
	ElasticIn,
	ElasticOut,
	ElasticHalfOut,
	ElasticQuarterOut,
	ElasticInOut,
	BackIn,
	BackOut, // 30
	BackInOut,
	BounceIn,
	BounceOut,
	BounceInOut,
	Count
};

extern std::string Easings[];

#endif//EASING_HPP