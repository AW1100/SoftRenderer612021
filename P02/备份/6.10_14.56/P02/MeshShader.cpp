#include "MeshShader.h"

MeshShader::MeshShader()
{

}

MeshShader::~MeshShader()
{

}

void MeshShader::setWorldViewProj(const Mat4& m)
{
	m_WVP = m;
}

void MeshShader::setWorld(const Mat4& m)
{
	m_world = m;
}

void MeshShader::setWorldInvTranspose(const Mat4& m)
{
	m_worldInvTranspose = m;
}

void MeshShader::setEyePos(const Vec4& eye)
{
	m_eyePos = eye;
}

VertexOut MeshShader::VS(const VertexIn& vin)
{
	VertexOut out;
	out.postPos = vin.pos*m_WVP;

	out.fragPos = vin.pos*m_world;
	out.normal = out.normal*m_worldInvTranspose;

	out.color = vin.color;
	
	return out;
}

Vec4 MeshShader::PS(VertexOut& pin)
{
	pin.normal.normalize();

	Vec4 toEye = (m_eyePos - pin.fragPos).normalize();

	return Vec4(0, 0, 0);
}