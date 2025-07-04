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
	Sphere sphere;
	sphere = { {0.0f, 0.0f, 0.0f}, 1.0f };

	// 平面
	Plane plane;
	plane = { {0.0f, 1.0f, 0.0f}, 1.0f };

	// 球の色
	unsigned int lineColor = WHITE;
	unsigned int planeColor = WHITE;

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

	ImGuiIO& io = ImGui::GetIO();

	float sensitivity = 0.01f;

	Segment segment = { {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };

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

		// 当たり判定
		bool isLineCollision = isCollisionLine(segment, plane);

		lineColor = (isLineCollision) ? RED : WHITE;

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
		Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドの表示
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), lineColor);
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, planeColor);

		// ImGuiの表示
		ImGui::Begin("Window");
		ImGui::DragFloat3("Plane.Normal", (float*)&plane.normal, 0.01f, -50, 50, "%0.3f");
		ImGui::DragFloat("Plane.distance", (float*)&plane.distance, 0.01f, -50, 50, "%0.3f");
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
