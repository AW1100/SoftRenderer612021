#include "Util.h"

const double Util::PI = 3.1415926535898;

UINT Util::colorToUINT(const Vec4& color)
{
	BYTE red = 255 * color.x;
	BYTE green = 255 * color.y;
	BYTE blue = 255 * color.z;
	return (UINT)((BYTE)blue | (WORD)((BYTE)green << 8) | (DWORD)((BYTE)red << 16));
}

Mat4 Util::screenTransform(int clientWidth, int clientHeight)
{
	return Mat4(
		clientWidth / 2, 0, 0, 0,
		0, clientHeight / 2, 0, 0,
		0, 0, 1, 0,
		clientWidth / 2, clientHeight / 2, 0, 1
	);
}



//矢量插值
Vec4 Util::Lerp(const Vec4& v1, const Vec4& v2, double t)
{
	return Vec4(
		Lerp(v1.x, v2.x, t),
		Lerp(v1.y, v2.y, t),
		Lerp(v1.z, v2.z, t),
		v1.w
	);
}

//Vec2 插值
Vec2 Util::Lerp(const Vec2& v1, const Vec2& v2, double t)
{
	return Vec2(
		Lerp(v1.x, v2.x, t),
		Lerp(v1.y, v2.y, t)
	);
}


//VertexOut 插值
VertexOut Util::Lerp(const VertexOut& v1, const VertexOut& v2, double t)
{
	return VertexOut(
		Lerp(v1.worldPos, v2.worldPos, t),
		Lerp(v1.fragPos, v2.fragPos, t),
		//Lerp(v1.tex, v2.tex, t),
		Lerp(v1.normal, v2.normal, t),
		Lerp(v1.color, v2.color, t),
		Lerp(v1.oneOverZ, v2.oneOverZ, t)
	);
}

double Util::clamp(double x, double min, double max)
{
	if (x <= min)
		return min;
	else if (x >= max)
		return max;
	return x;
}

Mat4 Util::getViewMatrix(Camera c)
{
	return Mat4(
		c.u.x, c.v.x, c.w.x, 0,
		c.u.y, c.v.y, c.w.y, 0,
		c.u.z, c.v.z, c.w.z, 0,
		-c.u.dot(c.eyePos), -c.v.dot(c.eyePos), -c.w.dot(c.eyePos), 1
	);
}

Mat4 Util::getProjectionMatrix(double fov, double aspect, double nearZ, double farZ)
{
	Mat4 mat(0.0);
	double height = cos(fov*0.5) / sin(fov*0.5);
	mat.m[0][0] = height / aspect;
	mat.m[1][1] = height;
	mat.m[2][2] = farZ / (farZ - nearZ);
	mat.m[2][3] = 1.0;
	mat.m[3][2] = (nearZ*farZ) / (nearZ - farZ);
	mat.m[3][3] = 0.0;
	return mat;
}

Mat4 Util::getScreenMappingMatrix(int w, int h)
{
	return Mat4(
		w / 2, 0, 0, 0,
		0, h / 2, 0, 0,
		0, 0, 1, 0,
		w / 2, h / 2, 0, 1
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
	double a1, double a2, double a3,
	double b1, double b2, double b3,
	double c1, double c2, double c3)
{
	return a1 * (b2*c3 - c2 * b3) - a2 * (b1*c3 - c1 * b3) + a3 * (b1*c2 - c1 * b2);
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


