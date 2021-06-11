#include "Vec4.h"

#include <math.h>



Vec4::Vec4(double x_, double y_, double z_, double w_)
{
	x = x_;
	y = y_;
	z = z_;
	w = w_;
}

Vec4::Vec4(const Vec4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
}

Vec4& Vec4::operator=(const Vec4& rhs)
{
	if (this == &rhs)
		return *this;
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}

Vec4 Vec4::operator+(const Vec4& v2) const
{
	return Vec4(x + v2.x, y + v2.y, z + v2.z);
}

Vec4 Vec4::operator-(const Vec4& v2) const
{
	return Vec4(x - v2.x, y - v2.y, z - v2.z);
}

Vec4 Vec4::operator*(double s)
{
	return Vec4(x*s, y*s, z*s);
}

Vec4 Vec4::operator*(const Mat4& m) const
{
	return Vec4(x*m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0],
		x*m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1],
		x*m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2],
		x*m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3]);
}


Vec4 Vec4::operator*(const Vec4& v2) const
{
	return Vec4(x*v2.x, y*v2.y, z*v2.z, w);
}

Vec4 Vec4::operator/(double s)
{
	return Vec4(x / s, y / s, z / s, w);
}

bool Vec4::operator==(const Vec4& v2) const
{
	return (x == v2.x && y == v2.y && z == v2.z);
}

double Vec4::dot(Vec4 v2) const
{
	return (x * v2.x + y * v2.y + z * v2.z);
}

Vec4 Vec4::cross(Vec4 v2) const
{
	return Vec4(y*v2.z - z * v2.y, z*v2.x - x * v2.z, x*v2.y - y * v2.x);
}

double Vec4::length() const
{
	return sqrt(x*x + y * y + z * z);
}

Vec4 Vec4::normalize()
{
	double temp = 1.0 / length();

	return Vec4(x*temp, y*temp, z*temp, w);
}

Vec4 Vec4::transform(const Mat4& m) const
{
	return Vec4(x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0],
		x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1],
		x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2],
		x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3]
	);
}
