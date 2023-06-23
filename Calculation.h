#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

#pragma region
struct Line // 直線
{
	Vector3 origin; //始点
	Vector3 diff;   //終点への差分ベクトル
};

struct Ray // 半直線
{
	Vector3 origin; //始点
	Vector3 diff;   //終点への差分ベクトル
};

struct Segment // 線分
{
	Vector3 origin; //始点
	Vector3 diff;   //終点への差分ベクトル
};
#pragma endregion

// 加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ（ノルム）
float Length(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// 積
Vector3 Multiply(const Vector3& v1, const Vector3& v2);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

//
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// アフィン変換行列
Matrix4x4
MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// ベクトル変換
Vector3 Transform(const Vector3& v, const Matrix4x4& m);

// 1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRation, float nearClip, float farClip);
// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// 3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

const int kRowHeight = 20;
const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, Matrix4x4 matrix, const char* label);

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

// 小さいほうを返す
float Min(float num, float min);
// 大きいほうを返す
float Max(float num, float max);

float Clamp(float num, float hight, float low);