#pragma once
#include "Shader.h"
#include "Mat4.h"
#include "Vec4.h"
#include "Vertex.h"
#include "Camera.h"
#include "Util.h"

#include <algorithm>
#include <random>

class Shader
{
public:
	Shader();
	~Shader();

	void setWorldViewProj(const Mat4& m);
	void setWorld(const Mat4& m);
	void setWorldInvTranspose(const Mat4& m);
	void setCamera(Camera c);

	VertexOut vertexShader(const VertexIn& vin);
	Vec4 pixelShader(VertexOut& pin);


public:
	Mat4 m_WVP;
	Mat4 m_world;
	Mat4 m_worldInvTranspose;
	
	Camera camera;
};