#pragma once

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

// 1. 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 2. 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 3. 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* name);
