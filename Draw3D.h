#pragma once
struct Sphere
{
	Vector3 center; // 中心
	float radius;   // 半径
};

struct Plane
{
	Vector3 normal; // 法線
	float distance; // 距離
};

struct Triangle {
	Vector3 vertices[3]; // 頂点
};

// 球同士の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

// 球と平画との衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

// 線と平面との衝突判定
bool IsCollision(const Segment& line, const Plane& plane);

// 線と三角形との衝突判定
bool IsCollision(const Segment& line, const Triangle& triangle);

// 球の描画 
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);
// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

// 10*10のグリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

Vector3 Perpendicular(const Vector3& vector);