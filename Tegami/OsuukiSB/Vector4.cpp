#include "Vector4.hpp"

Vector4::Vector4(Matrix m)
	: x(m[0][0]), y(m[1][0]), z(m[2][0]), w(m[3][0]) {

}

Vector4::Vector4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w) {
}