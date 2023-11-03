#pragma once
#include "Shader.h"
#include "Mat4.h"
#include "Vec4.h"
#include "Vertex.h"
#include "Camera.h"
#include "Util.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "Scene.h"

#include <algorithm>
#include <random>
#include <vector>
#include <memory>
#include <array>

using namespace Util;

class Shader
{
public:
	Shader();
	~Shader();

	void setWorld(const Mat4& m);
	void setCamera(const Camera& c);
	void setLight(const std::shared_ptr<Light> l);
	void setMaterial(const Material& m);
	void setTexture(Texture *tex);
	void setNormal(Texture *tex);

	inline void setViewMatrix(const Mat4& m) { m_viewMatrix = m; }
	inline void setProjectionMatrix(const Mat4& m) { m_projMatrix = m; }

	VertexOut vertexShader(const VertexIn& vin);
	Vec4 pixelShader(VertexOut& pin, double **depthMap);

	VertexOut shadowVertexShader(const VertexIn& vin);
	Vec4 shadowPixelShader(VertexOut& pin);

public:
	Mat4 m_model;

	Mat4 m_viewMatrix;
	Mat4 m_projMatrix;

	Mat4 m_lightSpaceMatrix;
	
	Camera m_camera;
	std::vector<std::shared_ptr<Light>> m_lights;
	Material m_material;
	RENDER_MODE render_mode;
	Texture *m_tex;
	Texture *m_norm;
	Object* m_obj;

	Vec4* m_directionalLightPos;
};