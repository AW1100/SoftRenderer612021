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
	camera = c;
}

VertexOut Shader::vertexShader(const VertexIn& vin)
{
	VertexOut out;
	out.postPos = vin.pos*m_WVP;

	out.fragPos = vin.pos*m_world;
	out.normal = out.normal*m_worldInvTranspose;

	out.color = vin.color;
	
	return out;
}

Vec4 Shader::pixelShader(VertexOut& pin)
{
	pin.normal = pin.normal.normalize();
	Vec4 lightColor(1.0, 1.0, 1.0);
	Vec4 toEye = (camera.eyePos - pin.fragPos).normalize();
	
	//Vec4 lightDir(1.0, 0.0, 0.0, 0.0);
	Vec4 ambient(0.1,0.1,0.1);

	Vec4 r = Util::reflect(toEye*(-1), pin.normal);
	double cosAlpha = Util::clamp(toEye.dot(r), 0, 1);

	double cosTheta = Util::clamp(toEye.dot(pin.normal), 0, 1);
	Vec4 result = lightColor * cosTheta + ambient + lightColor * pow(cosAlpha, 5);
	
	
	//Vec4 result = pin.color;
	//result = result * rand();
	return result;
}