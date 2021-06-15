#include "Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::setWorldViewProj(const Mat4& m)
{
	m_WVP = m;
}

void Shader::setWorld(const Mat4& m)
{
	m_world = m;
}

void Shader::setWorldInvTranspose(const Mat4& m)
{
	m_worldInvTranspose = m;
}

void Shader::setCamera(Camera c)
{
	m_camera = c;
}

void Shader::setLight(std::shared_ptr<Light> l)
{
	m_lights.push_back(l);
}

void Shader::setMaterial(Material& m)
{
	m_material = m;
}

VertexOut Shader::vertexShader(const VertexIn& vin)
{
	VertexOut out;
	out.fragPos = vin.pos * m_WVP;

	out.worldPos = vin.pos * m_world;
	out.normal = vin.normal * m_worldInvTranspose;
	out.color = vin.color;
	if (render_mode == RENDER_MODE::SOLID_NO_LIGHT)
		return out;
	out.normal = out.normal.normalize();
	Vec4 toEye = (m_camera.eyePos - out.worldPos).normalize();
	Vec4 res(0.0, 0.0, 0.0, 1.0);
	//Vec4 res(1.0, 1.0, 1.0, 1.0);
	
	for (int i = 0; i < m_lights.size(); i++)
	{
		res = res + m_lights[i]->computeLightColor(out,toEye,m_material);
	}
	res.w = out.color.w * m_material.diffuse.w;
	out.color = res;
	return out;
}

//VertexOut Shader::vertexShader(const VertexIn& vin)
//{
//	VertexOut out;
//	out.fragPos = vin.pos*m_WVP;
//
//	out.worldPos = vin.pos*m_world;
//	out.normal = vin.normal*m_worldInvTranspose;
//
//	out.color = vin.color;
//	
//	return out;
//}


Vec4 Shader::pixelShader(VertexOut& pin)
{
	
	return pin.color;
}

//Vec4 Shader::pixelShader(VertexOut& pin)
//{
//	if (render_mode == RENDER_MODE::SOLID_NO_LIGHT)
//		return pin.color;
//	pin.normal = pin.normal.normalize();
//	//Vec4 lightColor(1.0, 1.0, 1.0);
//	Vec4 toEye = (m_camera.eyePos - pin.worldPos).normalize();
//	Vec4 res(0.0, 0.0, 0.0, 1.0);
//	
//	for (int i = 0; i < m_lights.size(); i++)
//	{
//		res = res + m_lights[i]->computeLightColor(pin,toEye,m_material);
//	}
//	res.w = pin.color.w * m_material.diffuse.w;
//	return res;
//}
