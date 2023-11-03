#pragma once

class Vec3
{
public:
	Vec3() = default;
	Vec3(double x_, double y_, double z_);
	Vec3(const Vec3& rhs);
	Vec3& operator=(const Vec3& rhs);
	Vec3 operator+(const Vec3& v2) const;
	Vec3 operator-(const Vec3& v2) const;
	Vec3 operator*(double s);
	Vec3 operator*(const Vec3& v2) const;
	Vec3 operator/(double s);
	bool operator==(const Vec3& v2) const;
	double dot(Vec3 v2) const;
	Vec3 cross(Vec3 v2) const;
	double length() const;
	Vec3 normalize();

public:
	double x;
	double y;
	double z;
};