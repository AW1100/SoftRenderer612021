#include "Shader.h"

Shader::Shader()
{
	m_model = Mat4(0.0);
}

Shader::~Shader()
{

}


void Shader::setWorld(const Mat4& m)
{
	m_model = m;
}


void Shader::setCamera(const Camera& c)
{
	m_camera = c;
}

void Shader::setLight(std::shared_ptr<Light> l)
{
	m_lights.push_back(l);
}

void Shader::setMaterial(const Material& m)
{
	m_material = m;
}

void Shader::setTexture(Texture* tex)
{
	m_tex = tex;
}

void Shader::setNormal(Texture *tex)
{
	m_norm = tex;
}

VertexOut Shader::vertexShader(const VertexIn& vin)
{
	VertexOut out;

	//Mat4 MVP = m_projMatrix * m_viewMatrix * m_model;
	Mat3 modelInvTranspose = transposeMatrix(inverseMatrix(Mat3(m_model)));
	out.fragPos = m_model * vin.pos;
	out.fragPos = m_viewMatrix * out.fragPos;
	out.fragPos = m_projMatrix * out.fragPos;
	out.worldPos = m_model * vin.pos;
	out.normal = Mat4(modelInvTranspose) * vin.normal;

	out.color = vin.color;
	out.tex = vin.tex;

	Vec4 vtangent = vin.tangent;
	Vec4 vBitangent = vin.bitangent;
	Vec4 vNormal = vin.normal;
	
	// º∆À„tangent space matrix
	Mat3 normalMatrix = transposeMatrix(inverseMatrix(Mat3(m_model)));
	Vec4 t = (Mat4(normalMatrix) * vtangent).normalize();
	Vec4 b = (Mat4(normalMatrix) * vBitangent).normalize();
	Vec4 n = (Mat4(normalMatrix) * vNormal).normalize();
	
	out.vtangent = t;
	out.vbitanget = b;
	out.vnormal = n;

	out.fragPosLightSpace = m_lightSpaceMatrix * m_model * vin.pos;

	return out;
}

Vec4 Shader::pixelShader(VertexOut& pin, double **depthMap)
{
	if (render_mode == RENDER_MODE::COLOR)
	{
		return pin.color;
	}
	Vec2 coordinate = m_obj->tex->texture_buffer->sample(pin.tex);
	if (render_mode == RENDER_MODE::TEXTURE_NO_LIGHT)
	{
		if (m_obj->tex->texture_buffer != nullptr)
		{
			return m_obj->tex->texture_buffer->m_texMap[(int)coordinate.x][(int)coordinate.y];
		}
	}
	Vec4 texColor = pin.color;
	if (m_obj->tex->texture_buffer != nullptr)
	{
		texColor = m_obj->tex->texture_buffer->m_texMap[(int)coordinate.x][(int)coordinate.y];
	}
	
	if (m_obj->tex->normal_buffer != nullptr)
	{
		pin.normal = m_obj->tex->normal_buffer->m_texMap[(int)coordinate.x][(int)coordinate.y];
		Mat3 TBN(pin.vtangent, pin.vbitanget, pin.vnormal);
		pin.normal = (Mat4(TBN) * pin.normal).normalize();
	}
	
	Vec4 toEye = (m_camera.eyePos - pin.worldPos).normalize();
	Vec4 res(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < m_lights.size(); i++)
	{
		if (i == 1)
			continue;
		res = res + m_lights[i]->computeLightColor(pin, toEye, m_material, texColor, depthMap);
	}
	res = res * texColor;
	// ¥¶¿Ìout-of-gumat
	Util::clampToColor(res);
	return res;
}


VertexOut Shader::shadowVertexShader(const VertexIn& vin)
{
	VertexOut out;
	out.fragPos = m_lightSpaceMatrix * m_model * vin.pos;
	return out;
}

Vec4 Shader::shadowPixelShader(VertexOut& pin)
{
	return Vec4(0.0, 0.0, 0.0, 0.0);
}

