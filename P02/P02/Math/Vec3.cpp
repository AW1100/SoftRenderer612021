#include "Vec3.h"

#include <math.h>



Vec3::Vec3(double x_, double y_, double z_)
{
	x = x_;
	y = y_;
	z = z_;
}

Vec3::Vec3(const Vec3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

Vec3& Vec3::operator=(const Vec3& rhs)
{
	if (this == &rhs)
		return *this;
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

Vec3 Vec3::operator+(const Vec3& v2) const
{
	return Vec3(x + v2.x, y + v2.y, z + v2.z);
}

Vec3 Vec3::operator-(const Vec3& v2) const
{
	return Vec3(x - v2.x, y - v2.y, z - v2.z);
}

Vec3 Vec3::operator*(double s)
{
	return Vec3(x * s, y * s, z * s);
}


Vec3 Vec3::operator*(const Vec3& v2) const
{
	return Vec3(x * v2.x, y * v2.y, z * v2.z);
}

Vec3 Vec3::operator/(double s)
{
	return Vec3(x / s, y / s, z / s);
}

bool Vec3::operator==(const Vec3& v2) const
{
	return (x == v2.x && y == v2.y && z == v2.z);
}

double Vec3::dot(Vec3 v2) const
{
	return (x * v2.x + y * v2.y + z * v2.z);
}

Vec3 Vec3::cross(Vec3 v2) const
{
	return Vec3(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);
}

double Vec3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize()
{
	double temp = 1.0 / length();

	return Vec3(x * temp, y * temp, z * temp);
}
