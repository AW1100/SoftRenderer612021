#pragma once
#include "Vertex.h"
#include "Vec4.h"

class ShaderBase
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	virtual VertexOut VS(const VertexIn& vin) = 0;
	virtual Vec4 PS(VertexOut& pin) = 0;
};