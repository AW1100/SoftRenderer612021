#pragma once
#include "Vec4.h"

class Material
{
public:
	Material()
	{
		ambient = Vec4(0.1, 0.1, 0.1, 1.0);
		diffuse = Vec4(0.4, 0.4, 0.4, 1.0);
		specular = Vec4(0.9, 0.9, 0.9, 1.0);
	}

	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
};