#pragma once
#include "Vec4.h"

class Camera
{
public:
	Camera() = default;
	Camera(Vec4 eyePos_, Vec4 lookAt_, Vec4 up_);
private:
	void calculateUVW();

public:
	Vec4 eyePos;
	Vec4 lookAt;
	Vec4 up;
	Vec4 w;		//look
	Vec4 u;		//right
	Vec4 v;		//up
};