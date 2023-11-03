#pragma once
class Mat4;
class Vec4;
class Vec3;

class Mat3
{
public:
	Mat3() = default;
	Mat3(double m00, double m01, double m02,
		double m10, double m11, double m12,
		double m20, double m21, double m22);
	Mat3(double s);
	Mat3(const Mat4& m4);
	Mat3(const Vec4& v1, const Vec4& v2, const Vec4& v3);
	Mat3& operator= (const Mat3& rhs);
	Mat3 operator+(const Mat3& rhs);
	Mat3 operator-(const Mat3& rhs);
	Mat3 operator*(const Mat3& rhs);
	Vec3 operator*(const Vec3& rhs);
	Mat3 operator*(const double s) const;

public:
	double m[3][3];
};