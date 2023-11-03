#pragma once
#include "Vec4.h"

class Material
{
public:
	Material()
	{
		ambient = Vec4(0.7, 0.7, 0.7, 1.0);
		diffuse = Vec4(1.0, 1.0, 1.0, 1.0);
		specular = Vec4(1.3, 1.3, 1.3, 32.0);
	}

	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
};