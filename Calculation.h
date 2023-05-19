#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

class Calculation {
public:
	// 加算
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	// 減算
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	// 積
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	// 逆行列
	static Matrix4x4 Inverse(const Matrix4x4& m);
	// 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m);
	// 単位行列の作成
	static Matrix4x4 MakeIdentity4x4();

	// 加算
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	// 減算
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	// スカラー倍
	static Vector3 Multiply(float scalar, const Vector3& v);
	// 内積
	static float Dot(const Vector3& v1, const Vector3& v2);
	// 長さ（ノルム）
	static float Length(const Vector3& v);
	// 正規化
	static Vector3 Normlize(const Vector3& v);

	
};	

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, Matrix4x4 matrix, const char* label);

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);