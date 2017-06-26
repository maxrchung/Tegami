#include "Partgen.hpp"

PartGen::PartGen(std::string timeStart, std::string timeEnd, std::string directory, float r, float g, float b,float scale, float opacity)
{
	::Color color(r, g, b);
	Sprite sprite = Sprite(directory, Vector2::Zero, Layer::Foreground, Origin::Centre);
	sprite.Rotate(Time(timeStart).ms, Time(timeEnd).ms, 0, 100, Easing::Linear);
	sprite.Color(Time(timeStart).ms, Time(timeEnd).ms, color, color, Easing::Linear);
}

PartGen::~PartGen()
{
}
