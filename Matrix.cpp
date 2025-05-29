#include "Matrix.h"
#include <Novice.h>
#define _USE_MATH_DEFINES 
#include <cmath>


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
