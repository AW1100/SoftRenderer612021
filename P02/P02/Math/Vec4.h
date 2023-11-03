#pragma once

class Vec4
{
public:
	Vec4() = default;
	Vec4(double x_, double y_, double z_, double w_ = 1.0);
	Vec4(const Vec4& rhs);
	Vec4& operator=(const Vec4& rhs);
	Vec4 operator+(const Vec4& v2) const;
	Vec4 operator-(const Vec4& v2) const;
	Vec4 operator*(double s) const;
	Vec4 operator*(const Vec4& v2) const;
	Vec4 operator/(double s);
	bool operator==(const Vec4& v2) const;
	double dot(Vec4 v2) const;
	Vec4 cross(Vec4 v2) const;
	double length() const;
	Vec4 normalize();

public:
	double x;
	double y;
	double z;
	double w; // œÚ¡ø0   µ„1
};