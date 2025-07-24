#pragma once
#include <cstdint>

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Plane {
	Vector3 normal; //!< 法線
	float distance; //!< 距離
};

struct Triangle {
	Vector3 vertices[3];
	float distance;
};

struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);


// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

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

Vector3 Multiply(const float& scalor, const Vector3& v);

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

float Determinant3x3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33);

/// <summary>
/// グリッドを描画する関数
/// </summary>
/// <param name="viewProjectionMatrix">透視投影行列</param>
/// <param name="viewportMatrix">ビューポート</param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

/// <summary>
/// 球を描画する関数
/// </summary>
/// <param name="sphere">球のインスタンスの情報</param>
/// <param name="viewProjectionMatrix">透視投影行列</param>
/// <param name="viewportMatrix">ビューポート</param>
/// <param name="color">色</param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

Vector3 GetSpherePoint(float radius, float lat, float lon);

Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 正射影ベクトルを求める関数
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns></returns>
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 最近接点を求める関数
/// </summary>
/// <param name="point">ポイント</param>
/// <param name="segment">セグメント</param>
/// <returns></returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// 球の当たり判定を返す関数
/// </summary>
/// <param name="s1">球1</param>
/// <param name="s2">球2</param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment);

// ベクトルの長さを計算する関数
float Length(const Vector3& vector);

/// <summary>
/// ベクトルに直交するベクトルを計算する関数
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns></returns>
Vector3 Perpendicular(const Vector3& vector);

/// <summary>
/// 平面を描画する関数
/// </summary>
/// <param name="plane">平面の構造体データ</param>
/// <param name="viewProjectionMatrix">透視投影行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// 正規化する関数
/// </summary>
///<param name="normal">平面のベクトル</param>
/// <returns>正規化したベクトル</returns>
Vector3 Normalize(const Vector3& normal);

float Dot(const Vector3& v1, const Vector3& v2);

bool isCollisionLine(const Segment& line, const Plane& plane);

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// boxを描画する関数
/// </summary>
/// <param name="aabb">boxのminとmax</param>
/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawBox(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

/// <summary>
/// box同士の当たり判定を返す関数
/// </summary>
/// <param name="aabb1">ボックス1</param>
/// <param name="aabb2">ボックス2</param>
/// <returns>trueかfalseか</returns>
bool isCollisionBox(const AABB& aabb1, const AABB& aabb2);

/// <summary>
/// ボックスと球の当たり判定を返す関数
/// </summary>
/// <param name="aabb">ボックス</param>
/// <param name="sphere">球</param>
/// <returns>trueかfalseか</returns>
bool isCollisionBoxSphere(const AABB& aabb, const Sphere& sphere);

/// <summary>
/// aabbとsegmentの当たり判定を返す関数
///</summary>
///<param name="aabb">ボックス</param>
///<param name="segment">線</param>
///<returns>trueかfalseか</returns>
bool isCollision(const AABB& aabb, const Segment& segment);