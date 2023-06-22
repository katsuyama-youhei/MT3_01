#include <Novice.h>
#include <assert.h>
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Calculation.h"
#include "Draw3D.h"
#include<cmath>
#include <numbers>


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
	const uint32_t kSubdivision = 30; //分割数
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
	if (t < 0||t>1.0f) {
		return false;
	}
	return true;

};

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}