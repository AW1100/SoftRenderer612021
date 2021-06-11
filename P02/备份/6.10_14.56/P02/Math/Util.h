#pragma once
#include "Mat4.h"
#include "Vec4.h"
#include "Vec2.h"
#include "Vertex.h"
#include <math.h>
#include <cmath>
#include <windows.h>

namespace Util
{
	extern const double PI;

	UINT colorToUINT(const Vec4& color);
	Mat4 screenTransform(int clientWidth, int clientHeight);
	inline double Lerp(double x1, double x2, double t) { return x1 + (x2 - x1)*t; }
	Vec4 Lerp(const Vec4& v1, const Vec4& v2, double t);
	Vec2 Lerp(const Vec2& v1, const Vec2& v2, double t);
	VertexOut Lerp(const VertexOut& v1, const VertexOut& v2, double t);
	double clamp(double x, double min, double max);
	Mat4 getViewMatrix(Vec4 eyePos, Vec4 lookAt, Vec4 up);
	Mat4 getProjectionMatrix(double fov, double aspect, double nearZ, double farZ);
	Mat4 getScreenMappingMatrix(int w, int h);
	Mat4 transposeMatrix(const Mat4& m);
	double determinantMatrix(const Mat4& m);
	double adjElemMatrix(
		double a1, double a2, double a3,
		double b1, double b2, double b3,
		double c1, double c2, double c3);
	Mat4 adjMatrix(const Mat4& m);
	Mat4 inverseMatrix(const Mat4& m);
	inline double degreeToRadian(double degree) { return degree * (PI / 180.0); }

}
