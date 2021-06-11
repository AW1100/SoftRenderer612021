#include "Camera.h"

Camera::Camera(Vec4 eyePos_, Vec4 lookAt_, Vec4 up_)
{
	eyePos = eyePos_;
	lookAt = lookAt_;
	up = up_;
	calculateUVW();
}

void Camera::calculateUVW()
{
	w = (lookAt - eyePos).normalize();
	u = up.cross(w).normalize();
	v = w.cross(u);
}