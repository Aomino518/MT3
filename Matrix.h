#pragma once

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

/// <summary>
///  1. 平行移動行列関数
/// </summary>
/// <param name="translate">方向</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// 2. 拡大縮小行列関数
/// </summary>
/// <param name="scale">スケール</param>
/// <returns>拡大縮小行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// 3. 座標変換関数
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>(3+1)次元同次座標系</returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// 行列の数値を表示する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="matrix">行列</param>
/// <param name="name">行列名</param>
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);

/// <summary>
/// ベクトルの値を表示する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="vector">ベクトル</param>
/// <param name="name">ベクトル名</param>
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* name);
