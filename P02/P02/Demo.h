#pragma once
#include <windows.h>
#include <vector>

#include "Device.h"
#include "DeviceContext.h"
#include "Vertex.h"
#include "Shader.h"
#include "ObjReader.h"
#include "Light.h"
#include "Material.h"

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

	//����¼�����
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnWheelScroll(WPARAM p);
public:
	inline Device*& GetDevice() { return m_pDevice; }
private:
	int					m_width, m_height;
	HINSTANCE			m_hInstance;
	HWND				m_hWnd;

	Device*				m_pDevice;
	DeviceContext*		m_pImmediateContext;
	Shader*				m_pShader;

	Mat4				m_worldViewProj;			//�����ӽ�ͶӰ����
	Mat4				m_world;					//����任����
	Mat4				m_worldInvTranspose;		//����任������ת�� ���ڵ�������

	
	std::vector<Light>	m_Light;
	Material			material;

	//���������λ�ýǶȵ�
	float				m_theta;
	float				m_phi;
	float				m_radius;
	POINT				m_lastMousePos;
};

