#pragma once
class Rendering
{
	// 回転行列
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);

	//
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	// アフィン変換行列
	static Matrix4x4
		MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	// ベクトル変換
	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	// 1. 透視投影行列
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRation, float nearClip, float farClip);
	// 2. 正射影行列
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	// 3. ビューポート変換行列
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
};

