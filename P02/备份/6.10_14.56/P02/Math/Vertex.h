#pragma once
#include "Vec4.h"

class VertexIn
{
public:
	VertexIn() = default;
	VertexIn(Vec4 pos_, Vec4 color_, Vec4 normal_) :
		pos(pos_),
		color(color_),
		normal(normal_)
	{}

	VertexIn(const VertexIn& rhs) :
		pos(rhs.pos),
		color(rhs.color),
		normal(rhs.normal)
	{}

	VertexIn& operator= (const VertexIn& rhs)
	{
		this->pos = rhs.pos;
		this->color = rhs.color;
		this->normal = rhs.normal;

		return *this;
	}

public:
	Vec4 pos;
	Vec4 color;
	Vec4 normal;
};

class VertexOut
{
public:
	
	VertexOut() = default;
	VertexOut(Vec4 fragPos_, Vec4 postPos_, Vec4 normal_, Vec4 color_, double oneOverZ_) :
		fragPos(fragPos_),
		postPos(postPos_),
		normal(normal_),
		color(color_),
		oneOverZ(oneOverZ_)
	{}

	VertexOut(const VertexOut& rhs) :
		fragPos(rhs.fragPos),
		postPos(rhs.postPos),
		normal(rhs.normal),
		color(rhs.color),
		oneOverZ(rhs.oneOverZ)
	{}

	VertexOut& operator= (const VertexOut& rhs)
	{
		this->fragPos = rhs.fragPos;
		this->postPos = rhs.postPos;
		this->normal = rhs.normal;
		this->color = rhs.color;
		this->oneOverZ = rhs.oneOverZ;
		return *this;
	}

public:
	Vec4 fragPos;
	Vec4 postPos;
	Vec4 normal;
	Vec4 color;
	double oneOverZ;
};