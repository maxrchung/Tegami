#pragma once

#include <vector>
#include "Stroke.hpp"

class Character{
public:
	Character() {};
	Character(std::vector<Stroke> strokes);
	std::vector<Stroke> strokes;
};

