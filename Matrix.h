#pragma once

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

/// <summary>
/// 正射影行列の作成関数
/// </summary>
/// <param name="left">左上のX座標</param>
/// <param name="top">左上のY座標</param>
/// <param name="right">右下のX座標</param>
/// <param name="bottom">右下のY座標</param>
/// <returns>正射影行列</returns>
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// 透視投影行列の作成関数
/// </summary>
/// <param name="fovY">画角Y</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面への距離</param>
/// <param name="farClip">遠平面への距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// ビューポートに変換する関数
/// </summary>
/// <param name="left">左上のX座標</param>
/// <param name="top">左上のY座標</param>
/// <param name="width">右下のX座標</param>
/// <param name="height">右下のY座標</param>
/// <param name="minDepth">最小深度値</param>
/// <param name="maxDepth">最大深度値</param>
/// <returns>ビューポート行列</returns>
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

/// <summary>
/// 行列の積を求める関数
/// </summary>
/// <param name="m1">行列1</param>
/// <param name="m2">行列2</param>
/// <returns>行列の積</returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 平行移動行列関数
/// </summary>
/// <param name="translate">方向</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// 逆行列
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
Matrix4x4 Inverse(const Matrix4x4& matrix);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// クロス積を求める関数
/// </summary>
/// <param name="v1">1つ目のベクター</param>
/// <param name="v2">2つ目のベクター</param>
/// <returns>クロス積</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクターの数値を表示する関数
/// </summary>
/// <param name="x">X座標</param>
/// <param name="y">Y座標</param>
/// <param name="cross">表示したいベクター</param>
/// <param name="name">ベクター名</param>
/// <returns></returns>
void VectorScreenPrintf(int x, int y, Vector3& cross, const char* name);

float Determinant3x3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33);