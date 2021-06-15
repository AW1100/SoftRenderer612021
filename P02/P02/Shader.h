#pragma once
#include "Shader.h"
#include "Mat4.h"
#include "Vec4.h"
#include "Vertex.h"
#include "Camera.h"
#include "Util.h"

#include <algorithm>
#include <random>
#include <vector>
#include <memory>
#include "Light.h"
#include "Material.h"

class Shader
{
public:
	Shader();
	~Shader();

	void setWorldViewProj(const Mat4& m);
	void setWorld(const Mat4& m);
	void setWorldInvTranspose(const Mat4& m);
	void setCamera(Camera c);
	void setLight(std::shared_ptr<Light> l);
	void setMaterial(Material& m);

	VertexOut vertexShader(const VertexIn& vin);
	Vec4 pixelShader(VertexOut& pin);


public:
	Mat4 m_WVP;
	Mat4 m_world;
	Mat4 m_worldInvTranspose;
	
	Camera m_camera;
	std::vector<std::shared_ptr<Light>> m_lights;
	Material m_material;
	RENDER_MODE render_mode;
};