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

	//����¼�����
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

	Mat4						m_worldViewProj;			//�����ӽ�ͶӰ����
	Mat4						m_world;					//����任����
	Mat4						m_worldInvTranspose;		//����任������ת�� ���ڵ�������

	std::vector<VertexIn>		m_vertices;				//���㻺��
	std::vector<UINT>			m_indices;				//��������

	MeshGenerator::MeshData	m_box;
			//����

	//Lights::Material				m_material;				//����
	//Lights::DirectionalLight		m_dirLight;				//ƽ�й�Դ

	//���������λ�ýǶȵ�
	float						m_theta;
	float						m_phi;
	float						m_radius;
	POINT						m_lastMousePos;
};

