#include <Novice.h>
#include "Matrix.h"
#include <cstdint>
#define _USE_MATH_DEFINES 
#include <math.h>

const char kWindowTitle[] = "LE2C_25_モリ_アオト";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 v1{ 1.2f, -3.9f, 2.5f };
	Vector3 v2{ 2.8f, 0.4f, -1.3f };

	const Vector3 kLocalVertics[3] = {
		{0.0f, 1.0f, 0.0f}, // 上
		{1.0f, -1.0f, 0.0f}, // 右下
		{-1.0f, -1.0f, 0.0f} // 左下
	};

	Vector3 rotate{};
	Vector3 translate{0.0f, 0.0f, 6.0f};
	Vector3 cameraPosition{};

	static const int kWindowWidth = 1080;
	static const int kWindowHeight = 720;

	float speed = 0.1f;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (keys[DIK_W]) {
			translate.z += speed;
		}

		if (keys[DIK_S]) {
			translate.z -= speed;
		}

		if (keys[DIK_A]) {
			translate.x -= speed;
		}

		if (keys[DIK_D]) {
			translate.x += speed;
		}

		// Y軸を中心に回転
		rotate.y -= speed;

		// 角度が2π以上か0未満のときに戻す
		if (rotate.y > 2.0f * float(M_PI)) {
			rotate.y -= 2.0f * float(M_PI);
		} else if (rotate.y < 0.0f){
			rotate.y += 2.0f * float(M_PI);
		}

		// v1とv2の外積
		Vector3 cross = Cross(v1, v2);
		// ワールド行列の作成
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		// カメラの位置をワールド空間に変換する行列
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, cameraPosition);
		// ビュー行列はカメラ行列の逆行列
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		// 透視投影行列
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		// ワールドビュープロジェクション合成行列
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		// ビューポート変換
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertics[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 外積の数値表示
		VectorScreenPrintf(5, 0, cross, "Cross");
		// 三矩形の描画
		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
