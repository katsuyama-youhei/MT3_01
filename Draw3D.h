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

struct AABB {
	Vector3 min;
	Vector3 max;
};

// 球同士の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

// 球と平画との衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

// 線と平面との衝突判定
bool IsCollision(const Segment& line, const Plane& plane);

// 線と三角形との衝突判定
bool IsCollision(const Segment& line, const Triangle& triangle);

// AABB同士の衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

// AABBと球との衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);

// 球の描画 
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);
// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

// 10*10のグリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

// AABBの描画(矩形の立体)
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);

Vector3 Perpendicular(const Vector3& vector);

// minとmaxが入れ替わらないように
void SwapLimit(AABB& v);