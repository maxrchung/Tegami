#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include "Matrix.hpp"

class Vector4 {
public:
	explicit Vector4(Matrix m);
	Vector4(float x, float y, float z, float w);
	float x, y, z, w;
};

#endif//VECTOR4_HPP