#include "Matrix3D.h"
#include "Novice.h"

// 1.�s��̉��@
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = m1.m[j][i] + m2.m[j][i];
		}
	}
	return result;
}

// 2.�s��̌��@
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {

}

// 3.�s��̐�
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

}

// 4.�t�s��
Matrix4x4 Inverse(const Matrix4x4& m) {

}

// 5.�]�u�s��
Matrix4x4 Transpose(const Matrix4x4& m) {

}

// 6.�P�ʍs��̍쐬
Matrix4x4 MakeIdentity4x4() {

}

static const int kRowHeight = 20;
static const int kColumnWide = 60;
// 4x4�s��̐��l�\���֐�
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, char* name) {
	Novice::ScreenPrintf(x, y, "%c", &name);

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWide, 
				y + row * kRowHeight, 
				"%6.02f", 
				matrix.m[row][column]);
		}
	}
}