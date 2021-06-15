#pragma once
#include "Vec4.h"
#include "Vertex.h"
#include "Material.h"

class Light
{
public:
	Light() = default;
	Light(double s_) :
		strength(s_)
	{}
	virtual Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m) const = 0;
	virtual ~Light() = default;
	
public:
	double strength;
};

class Ambient : public Light
{
public:
	Ambient(double s_) :Light(s_)
	{}
	Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m) const override;

public:
	
};

class Directional : public Light
{
public:
	Directional(double s_, Vec4 d_) :Light(s_),
		direction(d_)
	{
		direction = direction.normalize();
		diffuse = Vec4(0.5, 0.5, 0.5, 1.0);
		specular = Vec4(0.7, 0.7, 0.7, 1.0);
	}
	Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m) const override;

public:
	Vec4 direction;
	Vec4 diffuse;
	Vec4 specular;
};