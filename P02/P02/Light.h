#pragma once
#include "Vec4.h"

class Light
{
public:
	
public:
	double strength;
	Vec4 diffuse;
	Vec4 specular;

};

class Ambient : public Light
{
public:

public:
	
};

class Directional : public Light
{
public:

public:
	Vec4 direction;
};