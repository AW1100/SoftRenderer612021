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

Vec4 Vec4::operator*(double s) const
{
	return Vec4(x*s, y*s, z*s);
}

Vec4 Vec4::operator*(const Vec4& v2) const
{
	return Vec4(x*v2.x, y*v2.y, z*v2.z, w);
}

Vec4 Vec4::operator/(double s)
{
	return Vec4(x / s, y / s, z / s, w / s);
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
	return Vec4(y*v2.z - z * v2.y, z*v2.x - x * v2.z, x*v2.y - y * v2.x, 0.0);
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
