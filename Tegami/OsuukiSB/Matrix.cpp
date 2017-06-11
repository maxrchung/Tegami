#include "Matrix.hpp"

Matrix::Matrix(int rows)
	: rows(rows), columns(rows), table(Table(rows, std::vector<float>(rows, 0))) {

}

Matrix::Matrix(int rows, int columns)
	: rows(rows), columns(columns), table(Table(rows, std::vector<float>(columns, 0))) {
}

Matrix::Matrix(Vector2 v)
	: rows(2), columns(1), table({ { v.x }, { v.y } }) {
}

Matrix::Matrix(Vector3 v)
	: rows(3), columns(1), table({ { v.x }, { v.y }, { v.z } }) {
}

Matrix::Matrix(Vector4 v)
	: rows(4), columns(1), table({ { v.x }, { v.y }, { v.z }, { v.w } }) {
}

Matrix::Matrix(TableInitializerList tableInit)
	: rows(tableInit.size()), columns(tableInit.begin()->size()) {
	table.insert(table.end(), tableInit.begin(), tableInit.end());
}

Matrix Matrix::operator*(const Matrix& rhs) {
	if (columns != rhs.rows) {
		throw "Cannot multiply matrices; incompatible sizes";
	}

	Matrix mult(rows, rhs.columns);
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < rhs.columns; ++j) {
			float value = 0;
			for (int k = 0; k < columns; ++k) {
				value += table[i][k] * rhs.table[k][j];
			}
			mult[i][j] = value;
		}
	}

	return mult;
}

void Matrix::operator=(const Table& table) {
	this->table = table;
	rows = table.size();
	columns = table[0].size();
}

bool Matrix::operator==(const Matrix& mat) {
	if (rows != mat.rows || columns != mat.columns) {
		return false;
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (table[i][j] != mat[i][j]) {
				return false;
			}
		}
	}

	return true;
}

std::vector<float>& Matrix::operator[](int row) {
	return table[row];
}

const std::vector<float>& Matrix::operator[](int row) const {
	return table[row];
}