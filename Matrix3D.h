#pragma once

struct Matrix4x4 {
	float m[4][4];
};

// 1.�s��̉��@
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 2.�s��̌��@
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 3.�s��̐�
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 4.�t�s��
Matrix4x4 Inverse(const Matrix4x4& m);
// 5.�]�u�s��
Matrix4x4 Transpose(const Matrix4x4& m);
// 6.�P�ʍs��̍쐬
Matrix4x4 MakeIdentity4x4();
// 4x4�s��̐��l�\���֐�
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, char* name);

