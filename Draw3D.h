#pragma once
struct Sphere
{
	Vector3 center; //中心
	float radius;   //半径
};

// 球同士の衝突判定
bool isCollision(const Sphere& s1, const Sphere& s2);

// 球の描画 
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

// 10*10のグリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);