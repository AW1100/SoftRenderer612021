#pragma once
#include <windows.h>
#include <vector>

#include "Device.h"
#include "DeviceContext.h"
#include "Vertex.h"
#include "MeshGenerator.h"
#include "MeshShader.h"

class Demo
{
public:
	Demo();
	~Demo();
public:
	bool init(HINSTANCE hInstance, HWND hWnd);
	void Update(double dt);
	void Render();
	void Clear();

	//鼠标事件控制
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnWheelScroll(WPARAM p);
public:
	inline Device*& GetDevice() { return m_pDevice; }
private:
	int							m_width, m_height;
	HINSTANCE					m_hInstance;
	HWND							m_hWnd;

	Device*				m_pDevice;
	DeviceContext*			m_pImmediateContext;
	MeshShader*					m_pShader;

	Mat4						m_worldViewProj;			//世界视角投影矩阵
	Mat4						m_world;					//世界变换矩阵
	Mat4						m_worldInvTranspose;		//世界变换逆矩阵的转置 用于调整法线

	std::vector<VertexIn>		m_vertices;				//顶点缓冲
	std::vector<UINT>			m_indices;				//索引缓冲

	MeshGenerator::MeshData	m_box;
			//纹理

	//Lights::Material				m_material;				//材质
	//Lights::DirectionalLight		m_dirLight;				//平行光源

	//控制摄像机位置角度等
	float						m_theta;
	float						m_phi;
	float						m_radius;
	POINT						m_lastMousePos;
};

