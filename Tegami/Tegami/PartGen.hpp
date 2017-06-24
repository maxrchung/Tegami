#pragma once

#include "Time.hpp"
#include "Color.hpp"
#include "Sprite.hpp"
#include "Easing.hpp"

class PartGen
{
public:
	PartGen(std::string timeStart, std::string timeEnd, std::string directory, int r, int g, int b, float scale, float opacity);
	~PartGen();
};

