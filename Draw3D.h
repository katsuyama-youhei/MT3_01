#pragma once

// 球
struct Sphere
{
	Vector3 center; // 中心
	float radius;   // 半径
};

// 平面
struct Plane
{
	Vector3 normal; // 法線
	float distance; // 距離
};

// 三角形
struct Triangle {
	Vector3 vertices[3]; // 頂点
};

// 平行な直方体
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

// AABBと線の衝突判定
bool IsCollision(const AABB& aabb, const Segment& line);

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

// 線形補完
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// ベジェ曲線上の点を求める
Vector3 Bezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,float t);

// ベジェ曲線の描画
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);