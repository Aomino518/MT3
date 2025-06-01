#include <Novice.h>
#include <imgui.h>
#include "Matrix.h"
#include <io.h>
#include <algorithm>

const char kWindowTitle[] = "LE2C_26_モリ_アオト";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// カメラの回転
	Vector3 cameraRotate{0.26f, 0.0f, 0.0f};
	// カメラの位置
	Vector3 cameraTranslate{0.0f, 1.9f, -6.49f};

	// 球の座標と半径
	Sphere sphere[2];
	sphere[0] = { {0.0f, 0.0f, 0.0f}, 1.0f };
	sphere[1] = { {1.0f, 1.0f, 0.0f}, 1.0f };

	// 球の色
	unsigned int color = WHITE;

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

	ImGuiIO& io = ImGui::GetIO();

	float sensitivity = 0.01f;

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

		// カメラの回転
		if (!ImGui::GetIO().WantCaptureMouse && io.MouseDown[1]) {
			cameraRotate.y += io.MouseDelta.x * sensitivity;
			cameraRotate.x += io.MouseDelta.y * sensitivity;

			cameraRotate.x = std::clamp(cameraRotate.x, -5.0f, 5.0f);
			cameraRotate.y = std::clamp(cameraRotate.y, -5.0f, 5.0f);
		}

		// カメラの位置の操作
		if (!ImGui::GetIO().WantCaptureMouse && io.MouseDown[0]) {
			cameraTranslate.x -= io.MouseDelta.x * sensitivity;
			cameraTranslate.y += io.MouseDelta.y * sensitivity;
			cameraTranslate.z += io.MouseWheel * 0.1f;
		}

		// カメラの位置をワールド空間に変換する行列
		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		// ビュー行列はカメラ行列の逆行列
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		// 透視投影行列
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		// ビュープロジェクション合成行列
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		// ビューポート変換
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// 球が当たっているか
		bool isSphereCollision = IsCollision(sphere[0], sphere[1]);

		// 当たっているときだけ赤に
		color = (isSphereCollision) ? RED : WHITE;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドの表示
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		for (int i = 0; i < 2; i++) {
			DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, color);
		}

		// ImGuiの表示
		ImGui::Begin("Window");
		ImGui::DragFloat3("Sphere[0].center", (float*)&sphere[0].center, 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat("Sphere[0].radius", (float*)&sphere[0].radius, 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("Sphere[1].center", (float*)&sphere[1].center, 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat("Sphere[1].radius", (float*)&sphere[1].radius, 0.01f, -50, 50, "%0.3f");
		ImGui::End();

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
