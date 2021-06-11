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
	pin.normal.normalize();
	Vec4 lightColor(1.0, 1.0, 1.0);
	Vec4 toEye = (camera.eyePos - pin.fragPos).normalize();

	double ambientStrength = 0.1;
	Vec4 ambient = lightColor * ambientStrength;

	Vec4 norm = pin.normal;
	Vec4 lightDir(0.0, 0.0, 1.0);
	double diff = max(pin.normal.dot(lightDir), 0.0);
	Vec4 diffuse = lightColor * diff;
	
	double specularStrength = 1.5;
	Vec4 viewDir = (camera.eyePos - pin.fragPos).normalize();
	Vec4 reflectDir = Util::reflect(lightDir*(-1), pin.normal);
	double spec = pow(max(viewDir.dot(reflectDir), 0.0), 64);
	Vec4 specular = lightColor * specularStrength * spec;

	Vec4 result = (ambient + diffuse + specular) * pin.color;


	return result;
}