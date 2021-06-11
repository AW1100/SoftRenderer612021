#pragma once

class Mat4
{
public:
	Mat4() = default;
	Mat4(double m00, double m01, double m02, double m03,
		double m10, double m11, double m12, double m13,
		double m20, double m21, double m22, double m23,
		double m30, double m31, double m32, double m33);
	Mat4(double s);
	Mat4& operator= (const Mat4& rhs);
	Mat4 operator+(const Mat4& rhs);
	Mat4 operator-(const Mat4& rhs);
	Mat4 operator*(const Mat4& rhs);

public:
	double m[4][4];
};