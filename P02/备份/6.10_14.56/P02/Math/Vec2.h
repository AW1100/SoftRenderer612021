#pragma once

class Vec2
{
public:
	Vec2() = default;
	Vec2(double x_, double y_);
	Vec2(const Vec2& rhs);
	Vec2& operator=(const Vec2& rhs);
	Vec2 operator-(const Vec2& rhs);
	Vec2 operator*(const float s);
	Vec2 operator+(const Vec2& rhs);

public:
	double x;
	double y;
};