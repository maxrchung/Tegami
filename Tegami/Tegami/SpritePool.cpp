#include "SpritePool.hpp"
#include <iostream>

SpritePool::SpritePool(std::string imagePath, Origin origin)
	: imagePath(imagePath), origin(origin) {

}

Sprite* SpritePool::Get(int index) {
	while (index >= sprites.size()) {
		Sprite* sprite = new Sprite(imagePath, Vector2::Zero, Layer::Foreground, origin);
		sprite->Fade(0, 0, 0, 0);
		sprites.push_back(sprite);
	}

	return sprites[index];
}

void SpritePool::ReGet() {
	getting = 0;
}

Sprite* SpritePool::Get() {
	return Get(getting++);
}