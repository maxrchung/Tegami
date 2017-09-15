#include "Storyboard.hpp"
#include <iostream>
#include <fstream>
#include <stddef.h>

Storyboard* Storyboard::instance = NULL;

Storyboard* Storyboard::Instance() {
	if (!instance) {
		instance = new Storyboard;
		for (int i = 0; i < Layer::LayerCount; ++i) {
			instance->sprites.push_back(std::vector<Sprite*>());
		}
	}
	return instance;
}

void Storyboard::Clear() {
	for (int i = 0; i < Layer::LayerCount; ++i) {
		instance->sprites.push_back(std::vector<Sprite*>());
	}
}

void Storyboard::Write(const std::string& destinationPath) {
	std::cout << "Writing to: " << destinationPath << std::endl;
	std::ofstream outputFile;
	outputFile.open(destinationPath);

	outputFile << "[Events]" << std::endl;
	outputFile << "//Background and Video events" << std::endl;

	for (int i = 0; i < Layer::LayerCount; ++i) {
		outputFile << "//Storyboard Layer " << i << " (" << Layers[i] << ")" << std::endl;
		for (auto sprite : sprites[i]) {
			sprite->Write(outputFile);
		}
	}

	outputFile << "//Storyboard Sound Samples" << std::endl;
	outputFile.close();
}
