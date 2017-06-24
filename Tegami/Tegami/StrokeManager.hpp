#pragma once
#include "Character.hpp"
#include <string>
#include <unordered_map>

class StrokeManager
{
public:
	StrokeManager(std::string directory);
	Character Get(std::string key);
private:
	std::unordered_map<std::string, Character> characters;
};