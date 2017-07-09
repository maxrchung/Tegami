#include "Rectangle.hpp"

Rectangle::Rectangle(Vector2 position, Vector2 size, float rotation) 
	: position(position), size(size), rotation(rotation) {
	center.x = position.x + size.x / 2;
	center.y = position.y - size.y / 2;
}