#pragma once
#include "Mat4.h"
#include "Vec4.h"
#include "Vec2.h"
#include "Vertex.h"
#include "Camera.h"
#include "Mat3.h"
#include <math.h>
#include <cmath>
#include <windows.h>

enum class RENDER_MODE { WIREFRAME, COLOR, TEXTURE_WITH_LIGHT, TEXTURE_NO_LIGHT };

namespace Util
{
	extern const double PI;

	inline double Lerp(double x1, double x2, double t) { return x1 + (x2 - x1) * t; }
	Vec4 Lerp(const Vec4& v1, const Vec4& v2, double t);
	Vec2 Lerp(const Vec2& v1, const Vec2& v2, double t);
	VertexOut Lerp(const VertexOut& v1, const VertexOut& v2, double t);
	UINT colorToUINT(const Vec4& color);
	double clamp(double x, double min, double max);
	void clampToColor(Vec4& color);
	Mat4 getViewMatrix(const Camera& c);
	Mat4 getPerspectiveProjectionMatrix(double fov, double aspect, double nearZ, double farZ);
	Mat4 getOrthographicProjectionMatrix(double size, double aspect, double nearZ, double farZ);
	Mat4 transposeMatrix(const Mat4& m);
	double determinantMatrix(const Mat4& m);
	double adjElemMatrix(
		double a1, double a2, double a3,
		double b1, double b2, double b3,
		double c1, double c2, double c3);
	Mat4 adjointMatrix(const Mat4& m);
	Mat4 inverseMatrix(const Mat4& m);
	inline double degreeToRadian(double degree) { return degree * (PI / 180.0); }
	Vec4 reflect(const Vec4& v1, const Vec4& v2);
	void vswap(VertexOut &v1, VertexOut &v2);

	Mat3 transposeMatrix(const Mat3& m);
	double determinantMatrix(const Mat3& m);
	double adjElemMatrix(
		double a1, double a2,
		double b1, double b2);
	Mat3 adjointMatrix(const Mat3& m);
	Mat3 inverseMatrix(const Mat3& m);

	Mat4 getTranslationMatrix(double x, double y, double z);
}
