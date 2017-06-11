#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>

enum Layer {
	Background,
	Fail,
	Pass,
	Foreground,
	LayerCount
};

extern std::string Layers[];

#endif//LAYER_HPP