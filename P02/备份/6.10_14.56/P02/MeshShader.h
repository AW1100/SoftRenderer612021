#pragma once
#include "ShaderBase.h"
#include "Mat4.h"
#include "Vec4.h"
#include "Vertex.h"

class MeshShader : public ShaderBase
{
public:
	MeshShader();
	~MeshShader();

	void setWorldViewProj(const Mat4& m);
	void setWorld(const Mat4& m);
	void setWorldInvTranspose(const Mat4& m);
	void setEyePos(const Vec4& eye);

	VertexOut VS(const VertexIn& vin);
	Vec4 PS(VertexOut& pin);


private:
	Mat4 m_WVP;

	Mat4 m_world;
	Mat4 m_worldInvTranspose;
	
	Vec4 m_eyePos;
};