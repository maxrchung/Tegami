#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "Vector3.hpp"
#include <functional>
#include <iostream>

class Vector2 {
public:
	static Vector2 Midpoint;
	static Vector2 ScreenSize;
	static Vector2 Zero;
	Vector2();
	Vector2(float x, float y);
	explicit Vector2(Vector3 v);
	// Creates a unit vector with a given rotation
	explicit Vector2(float rot);
	float Magnitude();
	Vector2 Normalize();
	float Dot(Vector2 v);
	float AngleBetween(Vector2 v);
	Vector2 Project(Vector2 axis);
	// Rotates around a specified point
	Vector2 RotateAround(Vector2 origin, float rotation);
	// Calls the above with Midpoint
	Vector2 Rotate(float rotation);

	Vector2 operator-();
	Vector2 operator+(Vector2 v);
	Vector2 operator-(Vector2 v);
	Vector2 operator*(float multiple);
	Vector2 operator*(Vector2 v);
	Vector2 operator/(float division);
	void operator+=(Vector2 v);
	void operator-=(Vector2 v);
	void operator*=(float multiple);
	void operator*=(Vector2 v);
	void operator/=(float division);
	bool operator==(Vector2 v);
	bool operator!=(Vector2 v);

	// For putting into sets?
	const bool operator<(const Vector2 v) const;
	const bool operator==(const Vector2 v) const;

	// put
	friend std::ostream& operator<<(std::ostream& os, const Vector2& vector);

	float x;
	float y;
};

// Hash(?)?
namespace std {
	template<>
	struct hash<Vector2> {
		size_t operator()(const Vector2& value) const {
			return (std::hash<float>()(value.x) * 0x1f1f1f1f) ^ (std::hash<float>()(value.y) << 1);
		}
	};
}

#endif//VECTOR2_HPP
