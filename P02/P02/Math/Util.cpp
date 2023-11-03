#include "Util.h"

const double Util::PI = 3.1415926535898;

//Vec4²åÖµ
Vec4 Util::Lerp(const Vec4& v1, const Vec4& v2, double t)
{
	return Vec4(
		Lerp(v1.x, v2.x, t),
		Lerp(v1.y, v2.y, t),
		Lerp(v1.z, v2.z, t),
		Lerp(v1.w, v2.w, t)
	);
}

//Vec2²åÖµ
Vec2 Util::Lerp(const Vec2& v1, const Vec2& v2, double t)
{
	return Vec2(
		Lerp(v1.x, v2.x, t),
		Lerp(v1.y, v2.y, t)
	);
}

VertexOut Util::Lerp(const VertexOut& v1, const VertexOut& v2, double t)
{
	return VertexOut(
		Lerp(v1.worldPos, v2.worldPos, t),
		Lerp(v1.fragPos, v2.fragPos, t),
		Lerp(v1.normal, v2.normal, t),
		Lerp(v1.color, v2.color, t),
		Lerp(v1.oneOverZ, v2.oneOverZ, t),
		Lerp(v1.tex, v2.tex, t),
		Lerp(v1.fragPosLightSpace, v2.fragPosLightSpace, t),
		Lerp(v1.soneOverZ, v2.soneOverZ, t),
		Lerp(v1.vtangent, v2.vtangent, t),
		Lerp(v1.vbitanget, v2.vbitanget, t),
		Lerp(v1.vnormal, v2.vnormal, t)
	);
}


UINT Util::colorToUINT(const Vec4& color)
{
	BYTE red = static_cast<BYTE>((int)(255 * color.x));
	BYTE green = static_cast<BYTE>((int)(255 * color.y));
	BYTE blue = static_cast<BYTE>((int)(255 * color.z));
	return (UINT)((BYTE)blue | (WORD)((BYTE)green << 8) | (DWORD)((BYTE)red << 16));
}

double Util::clamp(double x, double min, double max)
{
	if (x <= min)
		return min;
	else if (x >= max)
		return max;
	return x;
}

void Util::clampToColor(Vec4& color)
{
	double MAX = max(color.x, color.y);
	MAX = max(MAX, color.z);
	if (MAX > 1.0)
	{
		double b = 1.0 / MAX;
		color = color * b;
	}
}

Mat4 Util::getViewMatrix(const Camera& c)
{
	return Mat4(
		c.u.x, c.u.y, c.u.z, -c.u.dot(c.eyePos),
		c.v.x, c.v.y, c.v.z, -c.v.dot(c.eyePos),
		c.w.x, c.w.y, c.w.z, -c.w.dot(c.eyePos),
		0.0, 0.0, 0.0, 1.0
	);
}

Mat4 Util::getPerspectiveProjectionMatrix(double fov, double aspect, double nearZ, double farZ)
{
	double height = cos(fov*0.5) / sin(fov*0.5);
	return Mat4(height / aspect, 0.0, 0.0, 0.0,
		0.0, height, 0.0, 0.0,
		0.0, 0.0, farZ / (farZ - nearZ), -(nearZ * farZ) / (farZ - nearZ),
		0.0, 0.0, 1.0, 0.0);
}

Mat4 Util::getOrthographicProjectionMatrix(double size, double aspect, double nearZ, double farZ)
{
	return Mat4(1/(aspect*size), 0.0, 0.0, 0.0,
		0.0, 1/size, 0.0, 0.0,
		0.0, 0.0, 2 / (farZ - nearZ), (farZ + nearZ) / (farZ - nearZ),
		0.0, 0.0, 0.0, 1.0);
}

Mat3 Util::transposeMatrix(const Mat3& m)
{
	return Mat3(
		m.m[0][0], m.m[1][0], m.m[2][0],
		m.m[0][1], m.m[1][1], m.m[2][1],
		m.m[0][2], m.m[1][2], m.m[2][2]
	);
}

Mat4 Util::transposeMatrix(const Mat4& m)
{
	return Mat4(
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
	);
}

double Util::determinantMatrix(const Mat3& m)
{
	return (m.m[0][0]*m.m[1][1]*m.m[2][2]-m.m[0][0]*m.m[1][2]*m.m[2][1]-
		m.m[0][1]*m.m[1][0]*m.m[2][2]+m.m[0][1]*m.m[1][2]*m.m[2][0]+
		m.m[0][2]*m.m[1][0]*m.m[2][1]-m.m[0][2]*m.m[1][1]*m.m[2][0]
		);

}

double Util::determinantMatrix(const Mat4& m)
{
	return (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]
		);

}

double Util::adjElemMatrix(
	double a1, double a2,
	double b1, double b2)
{
	return  a1 * b2 - a2 * b1;
}


double Util::adjElemMatrix(
	double a1, double a2, double a3,
	double b1, double b2, double b3,
	double c1, double c2, double c3)
{
	return a1 * (b2*c3 - c2 * b3) - a2 * (b1*c3 - c1 * b3) + a3 * (b1*c2 - c1 * b2);
}

Mat3 Util::adjointMatrix(const Mat3& m)
{
	double a1 = adjElemMatrix(m.m[1][1], m.m[1][2], m.m[2][1], m.m[2][2]);
	double a2 = adjElemMatrix(m.m[1][0], m.m[1][2], m.m[2][0], m.m[2][2]);
	double a3 = adjElemMatrix(m.m[1][0], m.m[1][1], m.m[2][0], m.m[2][1]);
	double b1 = adjElemMatrix(m.m[0][1], m.m[0][2], m.m[2][1], m.m[2][2]);
	double b2 = adjElemMatrix(m.m[0][0], m.m[0][2], m.m[2][0], m.m[2][2]);
	double b3 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[2][0], m.m[2][1]);
	double c1 = adjElemMatrix(m.m[0][1], m.m[0][2], m.m[1][1], m.m[1][2]);
	double c2 = adjElemMatrix(m.m[0][0], m.m[0][2], m.m[1][0], m.m[1][2]);
	double c3 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[1][0], m.m[1][1]);

	Mat3 result(
		a1, -a2, a3,
		-b1, b2, -b3,
		c1, -c2, c3
	);
	return transposeMatrix(result);
}

Mat4 Util::adjointMatrix(const Mat4& m)
{
	double a1 = adjElemMatrix(m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]);
	double a2 = adjElemMatrix(m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]);
	double a3 = adjElemMatrix(m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]);
	double a4 = adjElemMatrix(m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]);
	double b1 = adjElemMatrix(m.m[0][1], m.m[0][2], m.m[0][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]);
	double b2 = adjElemMatrix(m.m[0][0], m.m[0][2], m.m[0][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]);
	double b3 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[0][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]);
	double b4 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[0][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]);
	double c1 = adjElemMatrix(m.m[0][1], m.m[0][2], m.m[0][3], m.m[1][1], m.m[1][2], m.m[1][3], m.m[3][1], m.m[3][2], m.m[3][3]);
	double c2 = adjElemMatrix(m.m[0][0], m.m[0][2], m.m[0][3], m.m[1][0], m.m[1][2], m.m[1][3], m.m[3][0], m.m[3][2], m.m[3][3]);
	double c3 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[0][3], m.m[1][0], m.m[1][1], m.m[1][3], m.m[3][0], m.m[3][1], m.m[3][3]);
	double c4 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[0][2], m.m[1][0], m.m[1][1], m.m[1][2], m.m[3][0], m.m[3][1], m.m[3][2]);
	double d1 = adjElemMatrix(m.m[0][1], m.m[0][2], m.m[0][3], m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3]);
	double d2 = adjElemMatrix(m.m[0][0], m.m[0][2], m.m[0][3], m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3]);
	double d3 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[0][3], m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3]);
	double d4 = adjElemMatrix(m.m[0][0], m.m[0][1], m.m[0][2], m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2]);

	Mat4 result(
		a1, -a2, a3, -a4,
		-b1, b2, -b3, b4,
		c1, -c2, c3, -c4,
		-d1, d2, -d3, d4
	);
	return transposeMatrix(result);
}

Mat3 Util::inverseMatrix(const Mat3& m)
{
	double det = std::abs(determinantMatrix(m));
	Mat3 adj = adjointMatrix(m);
	Mat3 inverse_m;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			inverse_m.m[i][j] = adj.m[i][j] / det;
		}
	}
	return inverse_m;
}

Mat4 Util::inverseMatrix(const Mat4& m)
{
	double det = std::abs(determinantMatrix(m));
	Mat4 adj = adjointMatrix(m);
	Mat4 inverse_m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverse_m.m[i][j] = adj.m[i][j] / det;
		}
	}
	return inverse_m;
}

Vec4 Util::reflect(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1 - v2 * (v1.dot(v2))*2.0);
}


void Util::vswap(VertexOut &v1, VertexOut &v2)
{
	VertexOut temp = v1;
	v1 = v2;
	v2 = temp;
}

Mat4 Util::getTranslationMatrix(double x, double y, double z)
{
	return Mat4(1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0);
}