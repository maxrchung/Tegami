#include "Vector2.hpp"
#include "Vector3.hpp"
#include "math.h"
#include <iostream>

Vector2 Vector2::Midpoint(320.0f, 240.0f);
Vector2 Vector2::ScreenSize(853.0f, 480.0f);
Vector2 Vector2::Zero(0.0f, 0.0f);

Vector2::Vector2() 
	: x(0), y(0) {
}

// Reverse y because of osu! coordinate system
Vector2::Vector2(float rotation) 
	: x(cos(rotation)), y(-sin(rotation)) {

}

Vector2::Vector2(float x, float y)
	: x(x), y(y) {
}

Vector2::Vector2(Vector3 v)
	: x(v.x), y(v.y) {
}

float Vector2::Magnitude() {
	return sqrt(x * x + y * y);
}

Vector2 Vector2::Normalize() {
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
}

float Vector2::Dot(Vector2 v) {
	return x * v.x + y * v.y;
}

float Vector2::AngleBetween(Vector2 v) {
	if (this->Magnitude() == 0 || v.Magnitude() == 0) {
		return 0;
	}

	Vector2 left = this->Normalize();
	Vector2 right = v.Normalize();

	if (left == right) {
		return 0;
	}

	float dot = left.Dot(right);

	// Floating points check
	if (dot > 1.0f) {
		dot = 1.0f;
	}
	else if (dot < -1.0f) {
		dot = -1.0f;
	}

	float rot = acos(dot);

	// http://stackoverflow.com/questions/11022446/direction-of-shortest-rotation-between-two-vectors
	// Use cross vector3 to determine direction
	Vector3 cross = Vector3(left.x, left.y).Cross(Vector3(right.x, right.y));
	if (cross.z > 0) {
		return -rot;
	}
	else {
		return rot;
	}
}

Vector2 Vector2::Project(Vector2 axis) {
	Vector2 projection = Vector2(this->Dot(axis), this->Dot(axis));
	projection /= (axis.x * axis.x) + (axis.y * axis.y);
	projection *= axis;
	return projection;
}

Vector2 Vector2::RotateAround(Vector2 origin, float rotation) {
	if (rotation == 0) {
		return *this;
	}

	Vector2 fromMid = *this - origin;
	Vector2 unitVec(1, 0);
	float angleFrom0 = fromMid.AngleBetween(unitVec);
	angleFrom0 -= rotation;
	Vector2 endMove(cos(angleFrom0), sin(angleFrom0));
	endMove *= fromMid.Magnitude();
	endMove = origin + endMove;

	return endMove;
}

Vector2 Vector2::Rotate(float rotation) {
	return RotateAround(Zero, rotation);
}

Vector2 Vector2::operator-() {
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+(Vector2 v) {
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(Vector2 v) {
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(float multiple) {
	return Vector2(x * multiple, y * multiple);
}

Vector2 Vector2::operator*(Vector2 v) {
	return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator/(float division) {
	return Vector2(x / division, y / division);
}

void Vector2::operator+=(Vector2 v) {
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(Vector2 v) {
	x -= v.x;
	y -= v.y;
}

void Vector2::operator*=(float multiple) {
	x *= multiple;
	y *= multiple;
}

void Vector2::operator*=(Vector2 v) {
	x *= v.x;
	y *= v.y;
}

void Vector2::operator/=(float division) {
	x /= division;
	y /= division;
}

bool Vector2::operator==(Vector2 v) {
	return x == v.x && y == v.y;
}

bool Vector2:: operator!=(Vector2 v) {
	return !(*this == v);
}

const bool Vector2::operator<(const Vector2 v) const {
	if (this->x < v.x) {
		return true;
	}
	else if (this->x == v.x && this->y < v.y) {
		return true;
	}
	else {
		return false;
	}
}

const bool Vector2::operator==(const Vector2 v) const {
	return x == v.x && y == v.y;
}

std::ostream& operator<<(std::ostream& os, const Vector2& vector)
{
	os << vector.x << " " << vector.y;
	return os;
}