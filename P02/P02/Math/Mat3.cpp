#include "Mat3.h"
#include "Mat4.h"
#include "Vec4.h"
#include "Vec3.h"

Mat3::Mat3(double m00, double m01, double m02,
	double m10, double m11, double m12,
	double m20, double m21, double m22)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;

}

Mat3::Mat3(double s)
{
	m[0][0] = s; m[0][1] = 0; m[0][2] = 0;
	m[1][0] = 0; m[1][1] = s; m[1][2] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = s;
}

Mat3::Mat3(const Mat4& m4)
{
	m[0][0] = m4.m[0][0]; m[0][1] = m4.m[0][1]; m[0][2] = m4.m[0][2];
	m[1][0] = m4.m[1][0]; m[1][1] = m4.m[1][1]; m[1][2] = m4.m[1][2];
	m[2][0] = m4.m[2][0]; m[2][1] = m4.m[2][1]; m[2][2] = m4.m[2][2];
}

Mat3::Mat3(const Vec4& v1, const Vec4& v2, const Vec4& v3)
{
	m[0][0] = v1.x; m[0][1] = v2.x; m[0][2] = v3.x;
	m[1][0] = v1.y; m[1][1] = v2.y; m[1][2] = v3.y;
	m[2][0] = v1.z; m[2][1] = v2.z; m[2][2] = v3.z;
	
}

Mat3& Mat3::operator= (const Mat3& rhs)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] = rhs.m[i][j];
		}
	}
	return *this;
}

Mat3 Mat3::operator+(const Mat3& rhs)
{
	Mat3 temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.m[i][j] = m[i][j] + rhs.m[i][j];
		}
	}

	return temp;
}

Mat3 Mat3::operator-(const Mat3& rhs)
{
	Mat3 temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.m[i][j] = m[i][j] - rhs.m[i][j];
		}
	}
	return temp;
}

Mat3 Mat3::operator*(const Mat3& rhs)
{
	Mat3 temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.m[i][j] = m[i][0] * rhs.m[0][j] +
				m[i][1] * rhs.m[1][j] +
				m[i][2] * rhs.m[2][j];
		}
	}

	return temp;
}

Vec3 Mat3::operator*(const Vec3& rhs)
{
	return Vec3(m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z,
				m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z,
				m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z);
}

Mat3 Mat3::operator*(const double s) const
{
	Mat3 t;
	t.m[0][0] *= s; t.m[0][1] *= s; t.m[0][2] *= s;
	t.m[1][0] *= s; t.m[1][1] *= s; t.m[1][2] *= s;
	t.m[2][0] *= s; t.m[2][1] *= s; t.m[2][2] *= s;
	return t;
}