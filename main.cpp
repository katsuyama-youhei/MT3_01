#include <Novice.h>
#include"Calculation.h"
#include"Rendering.h"

const char kWindowTitle[] = "LD2A_カツヤマヨウヘイ_確認課題";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int kWindowWidth = 1280, kWindowHeight = 720;
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	// クロス積の確認
	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Calculation::Cross(v1, v2);

	// 三角
	Vector3 cameraPosition{ 0.0f,0.0f,-500.0f };
	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,0.0f,0.0f };
	Vector3 kLocalVertices[3]{
		{0.0f,50.0f,0.0f},
		{50.0f,-50.0f,0.0f},
		{-50.0f,-50.0f,0.0f}
	};

	Matrix4x4 worldMatrix = Rendering::MakeAfineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
	Matrix4x4 cameraMatrix = Rendering::MakeAfineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
	Matrix4x4 viewMatrix = Calculation::Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = Rendering::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Calculation::Multiply(worldMatrix, Calculation::Multiply(viewMatrix, projectionMatrix));
	Matrix4x4 viewportMatrix = Rendering::MakeViewportMatrix(0, 0,float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
	Vector3 screenVertices[3];
	for (UINT32 i = 0; i < 3; ++i) {
		Vector3 ndcVertex = Rendering::TransformNormal(kLocalVertices[i], worldViewProjectionMatrix);
		screenVertices[i] = Rendering::TransformNormal(ndcVertex, viewportMatrix);
	}

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

		rotate.y += 0.02f;

		if (keys[DIK_W])
		{
			translate.z -= 2.0f;
		}
		else if (keys[DIK_S])
		{
			translate.z += 2.0f;
		}

		if (keys[DIK_A])
		{
			translate.x -= 2.0f;
		}
		else if (keys[DIK_D])
		{
			translate.x += 2.0f;
		}

		 worldMatrix = Rendering::MakeAfineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		 worldViewProjectionMatrix = Calculation::Multiply(worldMatrix, Calculation::Multiply(viewMatrix, projectionMatrix));
		for (UINT32 i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Rendering::TransformNormal(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Rendering::TransformNormal(ndcVertex, viewportMatrix);
		}

		Vector3 triangleCross = Calculation::Cross(Calculation::Subtract(screenVertices[1], screenVertices[0]), Calculation::Subtract(screenVertices[2], screenVertices[1]));

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, cross, "Cross");
		if (Calculation::Dot(cameraPosition, triangleCross) <= 0.0f) {
			Novice::DrawTriangle(
				int(screenVertices[0].x), int(screenVertices[0].y),
				int(screenVertices[1].x), int(screenVertices[1].y),
				int(screenVertices[2].x), int(screenVertices[2].y),
				RED, kFillModeSolid
			);
		}
	

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
