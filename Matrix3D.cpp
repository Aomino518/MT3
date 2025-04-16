#include "Matrix3D.h"
#include "Novice.h"

// 1.行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = m1.m[j][i] + m2.m[j][i];
		}
	}
	return result;
}

// 2.行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = m1.m[j][i] - m2.m[j][i];
		}
	}
	return result;
}

// 3.行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[j][i] += m1.m[j][k] * m2.m[k][i];
			}
		}
	}
	return result;
}

// 4.逆行列
/*Matrix4x4 Inverse(const Matrix4x4& matrix) {
	float det = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];

	if (det == 0) {
		Novice::ScreenPrintf(300, 300, "逆行列は存在しない");
		Matrix4x4 result = { 0 };
		return result;
	}

	Matrix4x4 result;
	float invDet = 1.0f / det;

	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) * invDet;
	result.m[0][1] = (matrix.m[0][2] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][2]) * invDet;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]) * invDet;

	result.m[1][0] = (matrix.m[1][2] * matrix.m[2][0] - matrix.m[1][0] * matrix.m[2][2]) * invDet;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]) * invDet;
	result.m[1][2] = (matrix.m[0][2] * matrix.m[1][0] - matrix.m[0][0] * matrix.m[1][2]) * invDet;

	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]) * invDet;
	result.m[2][1] = (matrix.m[0][1] * matrix.m[2][0] - matrix.m[0][0] * matrix.m[2][1]) * invDet;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]) * invDet;

	return result;
}*/

// 5.転置行列
Matrix4x4 Transpose(const Matrix4x4& matrix) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[j][i] = matrix.m[i][j];
		}
	}
	return result;
}

// 6.単位行列の作成
/*Matrix4x4 MakeIdentity4x4() {

}*/

static const int kRowHeight = 20;
static const int kColumnWide = 60;
// 4x4行列の数値表示関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name) {
	Novice::ScreenPrintf(x, y, "%s", name);

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWide,
				y + 20 + row * kRowHeight,
				"%6.02f",
				matrix.m[row][column]);
		}
	}
}