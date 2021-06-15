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
	void update(double dt);
	void Render();

	//����¼�����
	void onMouseDown(WPARAM btnState, int x, int y);
	void onMouseUp(WPARAM btnState, int x, int y);
	void onMouseMove(WPARAM btnState, int x, int y);
	void onWheelScroll(int p);
	void onKeyPressed(int f);
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

	//���������λ�ýǶȵ�
	double				m_theta;
	double				m_phi;
	double				m_radius;
	POINT				m_lastMousePos;
	Vec4				m_pos;
	Vec4				m_lookAt;
	double				m_zoomSpeed;
};

