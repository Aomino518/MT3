#include "Matrix.h"
#include <Novice.h>
#define _USE_MATH_DEFINES 
#include <cmath>
#include <algorithm>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

// 正射影行列の作成関数
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 orthoMatrix;

	orthoMatrix.m[0][0] = 2.0f / (right - left);
	orthoMatrix.m[0][1] = 0.0f;
	orthoMatrix.m[0][2] = 0.0f;
	orthoMatrix.m[0][3] = 0.0f;

	orthoMatrix.m[1][0] = 0.0f;
	orthoMatrix.m[1][1] = 2.0f / (top - bottom);
	orthoMatrix.m[1][2] = 0.0f;
	orthoMatrix.m[1][3] = 0.0f;

	orthoMatrix.m[2][0] = 0.0f;
	orthoMatrix.m[2][1] = 0.0f;
	orthoMatrix.m[2][2] = 1.0f / (farClip - nearClip);
	orthoMatrix.m[2][3] = 0.0f;

	orthoMatrix.m[3][0] = (left + right) / (left - right);
	orthoMatrix.m[3][1] = (top + bottom) / (bottom - top);
	orthoMatrix.m[3][2] = nearClip / (nearClip - farClip);
	orthoMatrix.m[3][3] = 1.0f;

	return orthoMatrix;
}


// 透視投影行列の作成関数
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 perspectiveFovMatrix;

	perspectiveFovMatrix.m[0][0] = (1.0f / aspectRatio) * 1.0f / tan(fovY / 2.0f);
	perspectiveFovMatrix.m[0][1] = 0.0f;
	perspectiveFovMatrix.m[0][2] = 0.0f;
	perspectiveFovMatrix.m[0][3] = 0.0f;

	perspectiveFovMatrix.m[1][0] = 0.0f;
	perspectiveFovMatrix.m[1][1] = 1.0f / tan(fovY / 2.0f);
	perspectiveFovMatrix.m[1][2] = 0.0f;
	perspectiveFovMatrix.m[1][3] = 0.0f;

	perspectiveFovMatrix.m[2][0] = 0.0f;
	perspectiveFovMatrix.m[2][1] = 0.0f;
	perspectiveFovMatrix.m[2][2] = farClip / (farClip - nearClip);
	perspectiveFovMatrix.m[2][3] = 1.0f;

	perspectiveFovMatrix.m[3][0] = 0.0f;
	perspectiveFovMatrix.m[3][1] = 0.0f;
	perspectiveFovMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	perspectiveFovMatrix.m[3][3] = 1.0f;

	return perspectiveFovMatrix;
}


// ビューポートに変換する関数
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 viewportMatrix;

	viewportMatrix.m[0][0] = width / 2.0f;
	viewportMatrix.m[0][1] = 0.0f;
	viewportMatrix.m[0][2] = 0.0f;
	viewportMatrix.m[0][3] = 0.0f;

	viewportMatrix.m[1][0] = 0.0f;
	viewportMatrix.m[1][1] = -(height / 2.0f);
	viewportMatrix.m[1][2] = 0.0f;
	viewportMatrix.m[1][3] = 0.0f;

	viewportMatrix.m[2][0] = 0.0f;
	viewportMatrix.m[2][1] = 0.0f;
	viewportMatrix.m[2][2] = maxDepth - minDepth;
	viewportMatrix.m[2][3] = 0.0f;

	viewportMatrix.m[3][0] = left + (width / 2.0f);
	viewportMatrix.m[3][1] = top + (height / 2.0f);
	viewportMatrix.m[3][2] = minDepth;
	viewportMatrix.m[3][3] = 1.0f;

	return viewportMatrix;
}

// 行列の積
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

Vector3 Multiply(const float& scalor, const Vector3& v) {
	Vector3 result{};
	result.x = v.x * scalor;
	result.y = v.y * scalor;
	result.z = v.z * scalor;
	return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = 0.0f;
		}
	}

	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1.0f;
	}

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 cofactor; // 余因子行列

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			float sub[3][3];
			int subi = 0;
			for (int i = 0; i < 4; ++i) {
				if (i == row) continue;
				int subj = 0;
				for (int j = 0; j < 4; ++j) {
					if (j == col) continue;
					sub[subi][subj] = matrix.m[i][j];
					++subj;
				}
				++subi;
			}

			// 小行列
			float det3 = Determinant3x3(
				sub[0][0], sub[0][1], sub[0][2],
				sub[1][0], sub[1][1], sub[1][2],
				sub[2][0], sub[2][1], sub[2][2]
			);

			cofactor.m[row][col] = ((row + col) % 2 == 0 ? 1 : -1) * det3;
		}
	}

	float det = 0.0f;
	for (int i = 0; i < 4; ++i) {
		det += matrix.m[0][i] * cofactor.m[0][i];
	}

	if (det == 0) {
		Novice::ScreenPrintf(300, 300, "逆行列は存在しない");
		Matrix4x4 result = { 0 };
		return result;
	}

	Matrix4x4 result;
	float invDet = 1.0f / det;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = cofactor.m[j][i] * invDet;
		}
	}

	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = 0.0f;
		}
	}

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;

	return result;
}

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	Matrix4x4 scaleRot = Multiply(scaleMatrix, rotateXYZMatrix);
	Matrix4x4 resultMatrix = Multiply(scaleRot, translateMatrix);

	return resultMatrix;
}

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	// wで割って正規化
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}

	return result;
}


// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 resultVector;
	resultVector.x = v1.y * v2.z - v1.z * v2.y;
	resultVector.y = v1.z * v2.x - v1.x * v2.z;
	resultVector.z = v1.x * v2.y - v1.y * v2.x;
	return resultVector;
}

float Determinant3x3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33) {
	return
		a11 * (a22 * a33 - a23 * a32) -
		a12 * (a21 * a33 - a23 * a31) +
		a13 * (a21 * a32 - a22 * a31);
}

// グリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; //分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 一つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t index = 0; index <= kSubdivision; ++index) {
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		float offset = -kGridHalfWidth + index * kGridEvery;
		// 奥から手前の始点と終点の座標
		Vector3 startX = { offset, 0.0f, -kGridHalfWidth }; // 手前
		Vector3 endX = { offset, 0.0f, kGridHalfWidth }; // 奥

		// 左右の始点と終点の座標
		Vector3 startZ = { -kGridHalfWidth, 0.0f, offset }; // 左
		Vector3 endZ = { kGridHalfWidth, 0.0f, offset }; // 右

		// スクリーン座標系まで変換をかける
		Vector3 startScreenX = Transform(Transform(startX, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreenX = Transform(Transform(endX, viewProjectionMatrix), viewportMatrix);

		Vector3 startScreenZ = Transform(Transform(startZ, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreenZ = Transform(Transform(endZ, viewProjectionMatrix), viewportMatrix);

		unsigned int color = (index == 5) ? 0x000000FF : 0xAAAAAAFF;

		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)
		Novice::DrawLine(
			static_cast<int>(startScreenX.x), static_cast<int>(startScreenX.y),
			static_cast<int>(endScreenX.x), static_cast<int>(endScreenX.y),
			color
		);

		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)
		Novice::DrawLine(
			static_cast<int>(startScreenZ.x), static_cast<int>(startScreenZ.y),
			static_cast<int>(endScreenZ.x), static_cast<int>(endScreenZ.y),
			color
		);
	}
}

// 球の描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 16; // 16分割
	const float kLonEvery = 2.0f * float(M_PI) / kSubdivision; // 経度
	const float kLatEvery = float(M_PI) / kSubdivision; // 緯度
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat =  -float(M_PI) / 2.0f + kLatEvery * latIndex;
		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度kLonEvery
			// world座標系でのa,b,cを求める
			Vector3 a, b, c;

			a = GetSpherePoint(sphere.radius, lat, lon);

			b = GetSpherePoint(sphere.radius, lat + kLatEvery, lon);

			c = GetSpherePoint(sphere.radius, lat, lon + kLonEvery);

			// a,b,cをScreen座標系まで変換
			Vector3 screenA = Transform(Transform(a + sphere.center, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b + sphere.center, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c + sphere.center, viewProjectionMatrix), viewportMatrix);
			
			// ab,acで線を引く
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);
		}
	}
}

Vector3 GetSpherePoint(float radius, float lat, float lon) {
	float x = radius * cosf(lat) * cosf(lon);
	float y = radius * sinf(lat);
	float z = radius * cosf(lat) * sinf(lon);
	return { x, y, z };
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

Vector3 operator*(const Vector3& v, float scalar) {
	return { v.x * scalar, v.y * scalar, v.z * scalar };
}

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	float ab = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	float b2 = (v2.x * v2.x) + (v2.y * v2.y) + (v2.z * v2.z);
	float scalar = ab / b2;
	result.x = scalar * v2.x;
	result.y = scalar * v2.y;
	result.z = scalar * v2.z;
	return result;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 result;
	Vector3 ab = segment.diff;
	Vector3 ap = point - segment.origin; // 終点 - 始点

	float abDot = (ab.x * ab.x) + (ab.y * ab.y) + (ab.z * ab.z);

	if (abDot == 0.0f) {
		return segment.origin;
	}

	float dot = (ap.x * ab.x) + (ap.y * ab.y) + (ap.z * ab.z);
	float t = dot / abDot;

	t = std::clamp(t, 0.0f, 1.0f);

	result.x = segment.origin.x + ab.x * t;
	result.y = segment.origin.y + ab.y * t;
	result.z = segment.origin.z + ab.z * t;

	return result;
}

// 球の当たり判定を返す関数
bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	const Vector3& v0 = triangle.vertices[0];
	const Vector3& v1 = triangle.vertices[1];
	const Vector3& v2 = triangle.vertices[2];

	// 三角形の法線
	Vector3 normal = Normalize(Cross(v1 - v0, v2 - v0));

	float dot = Dot(normal, segment.diff);
	if (std::abs(dot) < 1e-6f) {
		return false;
	}

	float d = Dot(normal, v0);
	float t = (d - Dot(normal, segment.origin)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 p = segment.origin + segment.diff * t;
 
	// 三角形の辺のベクトル
	Vector3 v01 = v1 - v0;
	Vector3 v12 = v2 - v1;
	Vector3 v20 = v0 - v2;

	Vector3 v0p = p - v0;
	Vector3 v1p = p - v1;
	Vector3 v2p = p - v2;


	Vector3 cross01 = Cross(v01, v0p);
	Vector3 cross12 = Cross(v12, v1p);
	Vector3 cross20 = Cross(v20, v2p);

	if (Dot(cross01, normal) >= 0.0f &&
		Dot(cross12, normal) >= 0.0f &&
		Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;
}

// ベクトルの長さを計算する関数を追加
float Length(const Vector3& vector) {
	return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

// ベクトルに直交するベクトルを計算する関数
Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

// 平面を描画する関数
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendculars[4];
	perpendculars[0] = Normalize(Perpendicular(plane.normal));
	perpendculars[1] = { -perpendculars[0].x, -perpendculars[0].y, -perpendculars[0].z };
	perpendculars[2] = Cross(plane.normal, perpendculars[0]);
	perpendculars[3] = { -perpendculars[2].x, -perpendculars[2].y, -perpendculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
}

// 正規化する関数
Vector3 Normalize(const Vector3& normal)
{
	float len = Length(normal);
	if (len == 0.0f) {
		return { 0.0f, 0.0f, 0.0f };
	}
	return { normal.x / len, normal.y / len, normal.z / len };
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

bool isCollisionLine(const Segment& line, const Plane& plane)
{
	float dot = Dot(plane.normal, line.diff);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 vertices[3];
	vertices[0] = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	vertices[1] = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	vertices[2] = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle(
		int(vertices[0].x),
		int(vertices[0].y),
		int(vertices[1].x),
		int(vertices[1].y),
		int(vertices[2].x),
		int(vertices[2].y),
		color,
		kFillModeWireFrame
		);
}

void DrawBox(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 vertices[8] = {
	{ aabb.min.x, aabb.min.y, aabb.min.z },
	{ aabb.max.x, aabb.min.y, aabb.min.z },
	{ aabb.min.x, aabb.max.y, aabb.min.z },
	{ aabb.max.x, aabb.max.y, aabb.min.z },
	{ aabb.min.x, aabb.min.y, aabb.max.z },
	{ aabb.max.x, aabb.min.y, aabb.max.z },
	{ aabb.min.x, aabb.max.y, aabb.max.z },
	{ aabb.max.x, aabb.max.y, aabb.max.z }
	};

	int indices[12][2] = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 奥面
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 手前面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 側面
	};

	Vector3 screen[8];
	for (int i = 0; i < 8; ++i) {
		screen[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 12; ++i) {
		const Vector3& p0 = screen[indices[i][0]];
		const Vector3& p1 = screen[indices[i][1]];
		Novice::DrawLine(int(p0.x), int(p0.y), int(p1.x), int(p1.y), color);
	}
}

bool isCollisionBox(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}

	return false;
}

bool isCollisionBoxSphere(const AABB& aabb, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
	};

	// 最近接点と球の中心との距離を求める
	float distance = Length(closestPoint - sphere.center);
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}


