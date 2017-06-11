#include "Matrix.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

Vector3 Vector3::Zero = Vector3(0, 0, 0);

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z) {
}

Vector3::Vector3(Vector2 v)
	: x(v.x), y(v.y), z(0) {

}

Vector3::Vector3(Matrix m)
	: x(m[0][0]), y(m[1][0]), z(m[2][0]) {
	if (m.columns != 1 || m.rows != 3) {
		throw "Cannot turn Matrix into a Vector3; Matrix is not a Vector3";
	}
}

float Vector3::AngleBetween(Vector3 v) {
	if (this->Magnitude() == 0 || v.Magnitude() == 0) {
		throw "Magnitude of a vector is 0";
	}
	else {
		float dotProd = this->Dot(v);
		dotProd /= this->Magnitude() * v.Magnitude();
		return acos(dotProd);
	}
}

Vector3 Vector3::Cross(Vector3 v) {
	return Vector3(y*v.z - z*v.y,
				   z*v.x - x*v.z,
				   x*v.y - y*v.x);
}

float Vector3::Dot(Vector3 v) {
	return x*v.x + y*v.y + z*v.z;
}

Vector3 Vector3::Project(Vector3 axis) {
	Vector3 projection = Vector3(this->Dot(axis), this->Dot(axis), this->Dot(axis));
	projection /= (axis.x * axis.x) + (axis.y * axis.y) + (axis.z * axis.z);
	projection *= axis;
	return projection;
}

float Vector3::Magnitude() {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize() {
	float len = this->Magnitude();
	return Vector3(x / len, y / len, z / len);
}

Vector3 Vector3::Rotate(float xRad, float yRad, float zRad) {
	// http://planning.cs.uiuc.edu/node102.html
	float cosx = cos(zRad);
	float cosy = cos(yRad);
	float cosz = cos(xRad);
	float sinx = sin(zRad);
	float siny = sin(yRad);
	float sinz = sin(xRad);
	Matrix rot = {
		{ cosx*cosy, cosx*siny*sinz - sinx*cosz, cosx*siny*cosz + sinx*sinz },
		{ sinx*cosy, sinx*siny*sinz + cosx*cosz, sinx*siny*cosz - cosx*sinz },
		{ -siny,     cosy*sinz,                  cosy*cosz                  },
	};

	return Vector3(rot * Matrix(*this));
}

Vector3 Vector3::RotateX(float xRad) {
	return Rotate(xRad, 0, 0);
}

Vector3 Vector3::RotateY(float yRad) {
	return Rotate(0, yRad, 0);
}

Vector3 Vector3::RotateZ(float zRad) {
	return Rotate(0, 0, zRad);
}

Vector3 Vector3::RotateAround(Vector3 v, float rotAmount) {
	v = v.Normalize();

	float r = rotAmount;

	// http://metalbyexample.com/linear-algebra/
	Matrix rot(3, 3);
	rot.table = {
		{ cos(r) + v.x*v.x*(1-cos(r)),     v.x*v.y*(1-cos(r)) - v.z*sin(r), v.x*v.z*(1-cos(r)) + v.y*sin(r) },
		{ v.y*v.x*(1-cos(r)) + v.z*sin(r), cos(r) + v.y*v.y*(1 - cos(r)),   v.y*v.z*(1-cos(r)) - v.x*sin(r) },
		{ v.z*v.x*(1-cos(r)) - v.y*sin(r), v.z*v.y*(1-cos(r)) + v.x*sin(r), cos(r) + v.z*v.z*(1-cos(r))     },
	};

	return Vector3(rot * Matrix(*this));
}

// https://www.youtube.com/watch?v=QR8bt3gwH58
Vector2 Vector3::Perspect(float cameraZ, float distance) {
	Matrix perspective(4, 4);
	perspective.table = {
		{ 1, 0, 0,			0 },
		{ 0, 1, 0,			0 },
		{ 0, 0, 1,			0 },
		{ 0, 0, -1/distance, 0 }
	};
	// Adjust for camera position
	Vector4 perspectee(this->x, this->y, this->z - cameraZ, 1);
	Vector4 perspected = Vector4(perspective * Matrix(perspectee));
	Vector2 perspectedDivided(perspected.x / perspected.w, perspected.y / perspected.w);

	return perspectedDivided;
}

Vector3 Vector3::operator-() {
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+(Vector3 rhs) {
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(Vector3 rhs) {
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator*(float rhs) {
	return Vector3(x * rhs, y * rhs, z * rhs);
}

Vector3 Vector3::operator/(float rhs) {
	return Vector3(x / rhs, y / rhs, z / rhs);
}

void Vector3::operator+=(Vector3 rhs) {
	*this = *this + rhs;
}

void Vector3::operator-=(Vector3 rhs) {
	*this = *this - rhs;
}

void Vector3::operator/=(float rhs) {
	*this = *this / rhs;
}

void Vector3::operator*=(Vector3 rhs) {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
}

bool Vector3::operator==(Vector3 rhs)
{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

void Vector3::operator*=(float rhs) {
	x *= rhs;
	y *= rhs;
	z *= rhs;
}