#include <Novice.h>
#include"Calculation.h"
#include"Draw3D.h"
#include <imgui.h>

const char kWindowTitle[] = "LD2A_カツヤマヨウヘイ_確認課題";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int kWindowWidth = 1280, kWindowHeight = 720;
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate = {};
	Vector3 translate = {};
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,1.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);

	Plane plane{ { 0.0f, 1.0f, 0.0f }, 1.0f };

	// 球のカラー変更用
	unsigned int color = 0xFFFFFFFF;


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

		Matrix4x4 worldMatrix = MakeAfineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAfineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldMViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		Vector3 start = Transform(Transform(segment.origin, worldMViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldMViewProjectionMatrix), viewportMatrix);

		if (IsCollision(segment, plane)) {
			color = 0xFF0000FF;
		}
		else {
			color = 0xFFFFFFFF;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldMViewProjectionMatrix, viewportMatrix);
		Novice::DrawLine(
			int(start.x), int(start.y),
			int(end.x), int(end.y),
			color
		);
		DrawPlane(plane, worldMViewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("SegmentOrigin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("SegmentDiff", &segment.diff.x, 0.01f);

		ImGui::DragFloat3("plane.Normal", &plane.normal.x, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::DragFloat("plane.Distance", &plane.distance, 0.01f);

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