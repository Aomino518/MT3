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

	// 平行移動
	Vector3 translates[3] = {
		{0.0f, 1.0f, 0.0f},
		{0.4f, 0.5f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};

	// 回転
	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, 0.0f},
	};

	// スケール
	Vector3 scales[3] = {
		{0.1f, 0.1f, 0.1f},
		{0.1f, 0.1f, 0.1f},
		{0.1f, 0.1f, 0.1f},
	};

	// 球の色
	uint32_t color[3] = {
		RED,
		GREEN,
		BLUE,
	};

	Sphere sphere[3] = {};

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

	ImGuiIO& io = ImGui::GetIO();

	// マウス操作速度の関数
	const float rotateSpeed = 0.01f;
	const float panSpeed = 0.01f;
	const float zoomSpeed = 0.1f;

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

		if (!ImGui::GetIO().WantCaptureMouse) {
			// 右クリックで回転
			if (io.MouseDown[1]) {
				cameraRotate.y += io.MouseDelta.x * rotateSpeed;
				cameraRotate.x += io.MouseDelta.y * rotateSpeed;

				cameraRotate.x = std::clamp(cameraRotate.x, -89.0f, 89.0f); // 俯瞰制限
			}

			// 中クリックで平行移動
			if (io.MouseDown[2]) {
				cameraTranslate.x -= io.MouseDelta.x * panSpeed;
				cameraTranslate.y += io.MouseDelta.y * panSpeed;
			}
			
			// ホイールでズーム
			if (io.MouseWheel != 0.0f) {
				cameraTranslate.z += io.MouseWheel * zoomSpeed;
			}
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

		// 肩のWorldMatrix
		Matrix4x4 shoulderWorldMatrix = MakeAffineMatrix(scales[0], rotates[0], translates[0]);

		// 肘のWorldMatrix
		Matrix4x4 elbowWorldMatrix = Multiply(MakeAffineMatrix(scales[1], rotates[1], translates[1]), shoulderWorldMatrix);

		// 手のWorldMatrix
		Matrix4x4 handWorldMatrix = Multiply(MakeAffineMatrix(scales[2], rotates[2], translates[2]), elbowWorldMatrix);

		// 座標変換
		Vector3 shoulderPos = Transform({ 0, 0, 0 }, shoulderWorldMatrix);
		Vector3 elbowPos = Transform({ 0, 0, 0 }, elbowWorldMatrix);
		Vector3 handPos = Transform({ 0, 0, 0 }, handWorldMatrix);

		sphere[0] = {shoulderPos, scales[0]};
		sphere[1] = {elbowPos, scales[1]};
		sphere[2] = {handPos, scales[2]};

		Vector3 sphere1 = Transform(Transform(sphere[0].center, viewProjectionMatrix), viewportMatrix);
		Vector3 sphere2 = Transform(Transform(sphere[1].center, viewProjectionMatrix), viewportMatrix);
		Vector3 sphere3 = Transform(Transform(sphere[2].center, viewProjectionMatrix), viewportMatrix);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドの表示
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		
		// 球の表示
		for (int i = 0; i < 3; i++) {
			DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, color[i]);
		}

		// 線の表示
		Novice::DrawLine(int(sphere1.x),
			int(sphere1.y),
			int(sphere2.x),
			int(sphere2.y),
			WHITE);

		Novice::DrawLine(int(sphere2.x),
			int(sphere2.y),
			int(sphere3.x),
			int(sphere3.y),
			WHITE);
		
		// ImGuiの表示
		ImGui::Begin("Window");
		ImGui::DragFloat3("translates[0]", (float*)&translates[0], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("rotates[0]", (float*)&rotates[0], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("scales[0]", (float*)&scales[0], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("translates[1]", (float*)&translates[1], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("rotates[1]", (float*)&rotates[1], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("scales[1]", (float*)&scales[1], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("translates[2]", (float*)&translates[2], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("rotates[2]", (float*)&rotates[2], 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat3("scales[2]", (float*)&scales[2], 0.01f, -50, 50, "%0.3f");
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
