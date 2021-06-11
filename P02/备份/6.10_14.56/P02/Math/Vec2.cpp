#include "Vec2.h"

Vec2::Vec2(double x_, double y_)
{
	x = x_;
	y = y_;
}

Vec2::Vec2(const Vec2& rhs)
{
	x = rhs.x;
	y = rhs.y;
}

Vec2& Vec2::operator=(const Vec2& rhs)
{
	if (this == &rhs)
		return *this;
	x = rhs.x;
	y = rhs.y;

	return *this;
}

Vec2 Vec2::operator-(const Vec2& rhs)
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float s)
{
	return Vec2(x*s, y*s);
}

Vec2 Vec2::operator+(const Vec2& rhs)
{
	return Vec2(x + rhs.x, y + rhs.y);
}