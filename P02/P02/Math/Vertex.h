#pragma once
#include "Vec4.h"
#include "Vec2.h"
#include "Mat3.h"

class VertexIn
{
public:
	VertexIn() = default;
	VertexIn(Vec4 pos_, Vec4 color_, Vec4 normal_, Vec2 tex_, Vec4 tangent_, Vec4 bitangent_) :
		pos(pos_),
		color(color_),
		normal(normal_),
		tex(tex_),
		tangent(tangent_),
		bitangent(bitangent_)
	{}

	VertexIn(const VertexIn& rhs) :
		pos(rhs.pos),
		color(rhs.color),
		normal(rhs.normal),
		tex(rhs.tex),
		tangent(rhs.tangent),
		bitangent(rhs.bitangent)
	{}

	VertexIn& operator= (const VertexIn& rhs)
	{
		this->pos = rhs.pos;
		this->color = rhs.color;
		this->normal = rhs.normal;
		this->tex = rhs.tex;
		this->tangent = rhs.tangent;
		this->bitangent = rhs.bitangent;

		return *this;
	}

public:
	Vec4 pos;
	Vec4 color;
	Vec4 normal;
	Vec2 tex;
	Vec4 tangent;
	Vec4 bitangent;
};

class VertexOut
{
public:
	
	VertexOut() = default;
	VertexOut(Vec4 worldPos_, Vec4 fragPos_, Vec4 normal_, Vec4 color_, double oneOverZ_, Vec2 tex_, Vec4 worldPosLightSpace_, double soneOverZ_, Vec4 t_, Vec4 b_, Vec4 n_) :
		worldPos(worldPos_),
		fragPos(fragPos_),
		normal(normal_),
		color(color_),
		oneOverZ(oneOverZ_),
		tex(tex_),
		fragPosLightSpace(worldPosLightSpace_),
		soneOverZ(soneOverZ_),
		vtangent(t_),
		vbitanget(b_),
		vnormal(n_)
	{}

	VertexOut(const VertexOut& rhs) :
		worldPos(rhs.worldPos),
		fragPos(rhs.fragPos),
		normal(rhs.normal),
		color(rhs.color),
		oneOverZ(rhs.oneOverZ),
		tex(rhs.tex),
		fragPosLightSpace(rhs.fragPosLightSpace),
		soneOverZ(rhs.soneOverZ),
		vtangent(rhs.vtangent),
		vbitanget(rhs.vbitanget),
		vnormal(rhs.vnormal)
	{}

	VertexOut& operator= (const VertexOut& rhs)
	{
		this->worldPos = rhs.worldPos;
		this->fragPos = rhs.fragPos;
		this->normal = rhs.normal;
		this->color = rhs.color;
		this->oneOverZ = rhs.oneOverZ;
		this->tex = rhs.tex;
		this->fragPosLightSpace = rhs.fragPosLightSpace;
		this->soneOverZ = rhs.soneOverZ;
		this->vtangent = rhs.vtangent;
		this->vbitanget = rhs.vbitanget;
		this->vnormal = rhs.vnormal;

		return *this;
	}

public:
	Vec4 worldPos;
	Vec4 fragPos;
	Vec4 normal;
	Vec4 color;
	double oneOverZ;
	Vec2 tex;
	Vec4 fragPosLightSpace;
	double soneOverZ;
	Vec4 vtangent;
	Vec4 vbitanget;
	Vec4 vnormal;
};