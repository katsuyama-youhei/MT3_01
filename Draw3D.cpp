#include <Novice.h>
#include <assert.h>
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Calculation.h"
#include "Draw3D.h"
#include<cmath>
#include <numbers>
#include <algorithm>
#include<stdint.h>


void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfwidth = 2.0f; //Gridの半分
	const uint32_t ksubdivsion = 10;   //分割数
	const float kGridEvery = (kGridHalfwidth * 2.0f) / float(ksubdivsion);	//１つ分の長さ

	Vector3 worldVertices[2];
	Vector3 screenVertices[2];
	Vector3 ndcVertex;

	//奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= ksubdivsion; ++xIndex) {
		//ワールド座標
		worldVertices[0] = { xIndex * kGridEvery - kGridHalfwidth,0.0f,kGridHalfwidth };
		worldVertices[1] = { xIndex * kGridEvery - kGridHalfwidth,0.0f, -kGridHalfwidth };
		//スクリーンへ変換
		for (uint32_t i = 0; i < 2; ++i) {
			ndcVertex = Transform(worldVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		if (xIndex * kGridEvery - kGridHalfwidth == 0.0f) {
			Novice::DrawLine(
				int(screenVertices[0].x), int(screenVertices[0].y),
				int(screenVertices[1].x), int(screenVertices[1].y),
				0x000000FF
			);
		}
		else {
			Novice::DrawLine(
				int(screenVertices[0].x), int(screenVertices[0].y),
				int(screenVertices[1].x), int(screenVertices[1].y),
				0xAAAAAAFF
			);
		}

	}

	//左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= ksubdivsion; ++zIndex) {
		//ワールド座標
		worldVertices[0] = { kGridHalfwidth,0.0f,zIndex * kGridEvery - kGridHalfwidth };
		worldVertices[1] = { -kGridHalfwidth,0.0f, zIndex * kGridEvery - kGridHalfwidth };
		//スクリーンへ変換
		for (uint32_t i = 0; i < 2; ++i) {
			ndcVertex = Transform(worldVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		if (zIndex * kGridEvery - kGridHalfwidth == 0.0f) {
			Novice::DrawLine(
				int(screenVertices[0].x), int(screenVertices[0].y),
				int(screenVertices[1].x), int(screenVertices[1].y),
				0x000000FF
			);
		}
		else {
			Novice::DrawLine(
				int(screenVertices[0].x), int(screenVertices[0].y),
				int(screenVertices[1].x), int(screenVertices[1].y),
				0xAAAAAAFF
			);
		}

	}

};

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {
	const uint32_t kSubdivision = 15; //分割数
	const float kLonEvery = 2.0f * float(std::numbers::pi) / float(kSubdivision);//経度分割1つ分の角度
	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision);//緯度分割1つ分の角度
	//緯度の方向に分割 
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -1.0f * float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;//現在の緯度
		//経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//現在の経度
			//world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = { sphere.radius * std::cosf(lat) * std::cosf(lon), sphere.radius * std::sinf(lat), sphere.radius * std::cosf(lat) * std::sinf(lon) };
			a = Add(a, sphere.center);
			b = { sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon), sphere.radius * std::sinf(lat + kLatEvery), sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon) };
			b = Add(b, sphere.center);
			c = { sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery), sphere.radius * std::sinf(lat), sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery) };
			c = Add(c, sphere.center);

			//a,b,cをスクリーン座標へ
			a = Transform(a, viewProjectionMatrix);
			a = Transform(a, viewportMatrix);
			b = Transform(b, viewProjectionMatrix);
			b = Transform(b, viewportMatrix);
			c = Transform(c, viewProjectionMatrix);
			c = Transform(c, viewportMatrix);


			//線を引く
			Novice::DrawLine(
				int(a.x), int(a.y),
				int(b.x), int(b.y),
				color
			);

			Novice::DrawLine(
				int(a.x), int(a.y),
				int(c.x), int(c.y),
				color
			);

		}
	}

};

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	//1.中心点を決める
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];

	//2.法線と垂直なベクトルを一つ求める
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));

	//3.2の逆ベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };

	//4.2の法線とのクロス積を求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);

	//5.4の逆ベクトルを求める
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	//6.2から5のベクトルを中心点にそれぞれ定数倍して足すと4頂点が出来上がる
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	//線を引く
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);

}

// 球同士の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2) {
	float distance = Length(Subtract(s2.center, s1.center));
	if (distance <= s1.radius + s2.radius) {
		return true;
	}
	return false;
};

// 球と平面との衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane) {
	float distance = std::fabsf(Dot(plane.normal, sphere.center) - plane.distance);
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
};

// 線と平面との衝突判定
bool IsCollision(const Segment& line, const Plane& plane) {

	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
	// tの値と線の種類によって衝突しているかを判断する
	if (t < 0 || t>1.0f) {
		return false;
	}
	return true;

};

// 線と三角形との衝突判定
bool IsCollision(const Segment& line, const Triangle& triangle) {

	// 各辺を結んだベクトル
	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	//　平面を求める
	Plane plane = { Normalize(Cross(v01, v12)),  0.0f };
	plane.distance = plane.normal.x * v20.x + plane.normal.y * v20.y + plane.normal.z * v20.z;

	//平面との衝突確認
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	if (t < 0 || t > 1.0f) {
		return false;
	}

	// 衝突点pを求める
	Vector3 p = Add(line.origin, Multiply(t, line.diff));

	// 衝突点pと角頂点を結んだベクトル
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);

	// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積をとる
	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	// 全小三角形のクロス積と法線が同じ方向を向いていたら衝突
	// 0は内側と判断
	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;
};

// AABB同士の衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	return false;
};

// AABBと球との衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	//最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y) ,
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

	//最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;
};

// AABBと線の衝突判定
bool IsCollision(const AABB& aabb, const Segment& line) {
	Vector3 tMin = {
		(aabb.min.x - line.origin.x) / line.diff.x,
		(aabb.min.y - line.origin.y) / line.diff.y,
		(aabb.min.z - line.origin.z) / line.diff.z
	};

	Vector3 tMax = {
		(aabb.max.x - line.origin.x) / line.diff.x,
		(aabb.max.y - line.origin.y) / line.diff.y,
		(aabb.max.z - line.origin.z) / line.diff.z
	};

	Vector3 tNear = {
		(std::min)(tMin.x, tMax.x) ,
		(std::min)(tMin.y, tMax.y) ,
		(std::min)(tMin.z, tMax.z)
	};

	Vector3 tFar = {
	(std::max)(tMin.x, tMax.x) ,
	(std::max)(tMin.y, tMax.y) ,
	(std::max)(tMin.z, tMax.z)
	};

	// AABBとの衝突点（貫通点）のtが小さいほう
	float tMin_ = (std::max)((std::max)(tNear.x, tNear.y), tNear.z);
	// AABBとの衝突点（貫通点）のtが大きいほう
	float tMax_ = (std::min)((std::min)(tFar.x, tFar.y), tFar.z);

	if (tMin_ <= tMax_) {

		if (tMin_ < 1.0f && tMax_ > 0.0f) {
			return true;
		}
	}

	return false;
};

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {
	Triangle triangle_{
		Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix),
		Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix),
		Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix)
	};

	Novice::DrawTriangle(
		int(triangle_.vertices[0].x), int(triangle_.vertices[0].y),
		int(triangle_.vertices[1].x), int(triangle_.vertices[1].y),
		int(triangle_.vertices[2].x), int(triangle_.vertices[2].y),
		color, kFillModeWireFrame
	);
};

// AABBの描画(矩形の立体)
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	// min/maxで8頂点を求める
	Vector3 frontLeftBottom = aabb.min;
	Vector3 frontLeftTop = { aabb.min.x,aabb.max.y,aabb.min.z };
	Vector3 frontRightTop = { aabb.max.x,aabb.max.y,aabb.min.z };
	Vector3 frontRightBottom = { aabb.max.x,aabb.min.y,aabb.min.z };

	Vector3 backRightTop = aabb.max;
	Vector3 backLeftTop = { aabb.min.x,aabb.max.y,aabb.max.z };
	Vector3 backLeftBottom = { aabb.min.x, aabb.min.y, aabb.max.z };
	Vector3 backRightBottom = { aabb.max.x,aabb.min.y,aabb.max.z };

	frontLeftBottom = Transform(Transform(frontLeftBottom, viewProjectionMatrix), viewportMatrix);
	frontLeftTop = Transform(Transform(frontLeftTop, viewProjectionMatrix), viewportMatrix);
	frontRightTop = Transform(Transform(frontRightTop, viewProjectionMatrix), viewportMatrix);
	frontRightBottom = Transform(Transform(frontRightBottom, viewProjectionMatrix), viewportMatrix);

	backRightTop = Transform(Transform(backRightTop, viewProjectionMatrix), viewportMatrix);
	backLeftTop = Transform(Transform(backLeftTop, viewProjectionMatrix), viewportMatrix);
	backLeftBottom = Transform(Transform(backLeftBottom, viewProjectionMatrix), viewportMatrix);
	backRightBottom = Transform(Transform(backRightBottom, viewProjectionMatrix), viewportMatrix);

	// 8頂点を結んで線を引く
	Novice::DrawLine(int(frontLeftBottom.x), int(frontLeftBottom.y), int(frontLeftTop.x), int(frontLeftTop.y), color);
	Novice::DrawLine(int(frontLeftBottom.x), int(frontLeftBottom.y), int(frontRightBottom.x), int(frontRightBottom.y), color);
	Novice::DrawLine(int(frontLeftBottom.x), int(frontLeftBottom.y), int(backLeftBottom.x), int(backLeftBottom.y), color);

	Novice::DrawLine(int(backLeftTop.x), int(backLeftTop.y), int(backLeftBottom.x), int(backLeftBottom.y), color);
	Novice::DrawLine(int(backLeftTop.x), int(backLeftTop.y), int(backRightTop.x), int(backRightTop.y), color);
	Novice::DrawLine(int(backLeftTop.x), int(backLeftTop.y), int(frontLeftTop.x), int(frontLeftTop.y), color);

	Novice::DrawLine(int(frontRightTop.x), int(frontRightTop.y), int(frontLeftTop.x), int(frontLeftTop.y), color);
	Novice::DrawLine(int(frontRightTop.x), int(frontRightTop.y), int(frontRightBottom.x), int(frontRightBottom.y), color);
	Novice::DrawLine(int(frontRightTop.x), int(frontRightTop.y), int(backRightTop.x), int(backRightTop.y), color);

	Novice::DrawLine(int(backRightBottom.x), int(backRightBottom.y), int(backRightTop.x), int(backRightTop.y), color);
	Novice::DrawLine(int(backRightBottom.x), int(backRightBottom.y), int(backLeftBottom.x), int(backLeftBottom.y), color);
	Novice::DrawLine(int(backRightBottom.x), int(backRightBottom.y), int(frontRightBottom.x), int(frontRightBottom.y), color);

};

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

void SwapLimit(AABB& aabb) {
	aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
	aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);

	aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
	aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);

	aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
	aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
};

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return { (1.0f - t) * v1.x + t * v2.x,(1.0f - t) * v1.y + t * v2.y,(1.0f - t) * v1.z + t * v2.z };
};

Vector3 Bezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, float t) {
	Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t);
	Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, t);
	Vector3 p = Lerp(p0p1, p1p2, t);
	return p;
};

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {
	// 分割数
	constexpr int kNumDevide = 16;

	for (int divideIndex = 0; divideIndex < kNumDevide; ++divideIndex) {
		float t = float(divideIndex) / float(kNumDevide);
		float nextT = float(divideIndex + 1) / float(kNumDevide);

		// 2次ベジェ曲線上の点を求める
		Vector3 p = Bezier(controlPoint0, controlPoint1, controlPoint2, t);
		Vector3 pNext = Bezier(controlPoint0, controlPoint1, controlPoint2, nextT);

		p = Transform(Transform(p, viewProjectionMatrix), viewportMatrix);
		pNext = Transform(Transform(pNext, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(p.x), int(p.y), int(pNext.x), int(pNext.y), color);
	}

};