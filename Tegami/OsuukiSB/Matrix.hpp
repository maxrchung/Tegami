#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <vector>

typedef std::vector<std::vector<float>> Table;
typedef std::initializer_list<std::initializer_list<float>> TableInitializerList;

class Vector4;
class Matrix {
public:
	explicit Matrix(Vector2 v);
	explicit Matrix(Vector3 v);
	explicit Matrix(Vector4 v);
	explicit Matrix(int rows);
	Matrix(TableInitializerList tableInit);
	Matrix(int rows, int columns);

	Matrix operator*(const Matrix& rhs);
	void operator=(const Table& table);
	bool operator==(const Matrix& mat);
	std::vector<float>& operator[](int row);
	const std::vector<float>& operator[](int row) const;

	int rows;
	int columns;

	Table table;
};

#endif//MATRIX_HPP