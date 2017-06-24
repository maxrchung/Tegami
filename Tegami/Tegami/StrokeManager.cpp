#include "StrokeManager.hpp"
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = std::experimental::filesystem;

StrokeManager::StrokeManager(std::string directory) {
	for (auto& dirEntry : fs::recursive_directory_iterator(directory)) {
		std::stringstream ss;
		ss << dirEntry;

		std::string strokeImage = ss.str();
		std::ifstream file(strokeImage);

		char noStrokes;
		file >> noStrokes;
		std::string line;
		
		while (std::getline(file, line)) {
			std::cout << line << std::endl;
		}
		
	
	}
 }

Character StrokeManager::Get(std::string key) {
	return characters[key];
}