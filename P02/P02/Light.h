#pragma once
#include "Vec4.h"
#include "Vertex.h"
#include "Material.h"
#include "Scene.h"
#include <array>

class Light
{
public:
	Light() = default;

	virtual Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double **depthMap) = 0;
	virtual ~Light() = default;
};

class Ambient : public Light
{
public:
	Ambient(double s_):
		strength(s_)
	{}

	Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double** depthMap) override;

public:
	double strength;
};

class Directional : public Light
{
public:
	Directional(Vec4 d_) :
		direction(d_)
	{
		direction = direction.normalize();
		diffuse = Vec4(0.8, 0.8, 0.8, 1.0);
		specular = Vec4(0.9, 0.9, 0.9, 1.0);
	}
	Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double** depthMap) override;

	double shadowCalculation(Vec4& worldPosLightSpace, double** depthMap, double soneOverZ, double bias);

public:
	Vec4 direction;
	Vec4 diffuse;
	Vec4 specular;
};

class PointLight : public Light
{
public:
	PointLight(Vec4 position_) :
		position(position_)
	{
		diffuse = Vec4(0.8, 0.8, 0.8, 1.0);
		specular = Vec4(0.9, 0.9, 0.9, 1.0);

		constant = 1.0;
		linear = 0.09;
		quadratic = 0.032;
	}

	PointLight(Vec4 position_, Object* o_) :
		position(position_)
	{
		diffuse = Vec4(0.8, 0.8, 0.8, 1.0);
		specular = Vec4(0.9, 0.9, 0.9, 1.0);

		constant = 1.0;
		linear = 0.14;
		quadratic = 0.07;
		lightModel = o_;
	}

	inline void setModel(Object* mod) { lightModel = mod; }

	Vec4 computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double** depthMap) override;

	double shadowCalculation(Vec4& worldPosLightSpace, double** depthMap, double soneOverZ, double bias);

public:
	Vec4 position;
	Vec4 diffuse;
	Vec4 specular;

	double constant;
	double linear;
	double quadratic;

	Object* lightModel;
};