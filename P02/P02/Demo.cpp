#include <vector>
#include "Demo.h"
#include "Shader.h"
#include "Util.h"
#include "ObjReader.h"
#include "Camera.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace Util;

Demo::Demo() :m_theta(1.5f * PI), m_phi(0.4*PI), m_radius(5.0f)
{
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;

	m_world = Mat4(1.0);

}


Demo::~Demo()
{
	Clear();
}

bool Demo::init(HINSTANCE hInstance, HWND hWnd)
{
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	m_pDevice = new Device(m_width, m_height);
	m_pImmediateContext = new DeviceContext();
	m_pImmediateContext->init(m_pDevice);
	m_pShader = new Shader();

	// 读取obj文件
	std::string filePath = "../deer.obj";
	ObjReader* obj = new ObjReader;
	obj->readObjectFile(filePath);
	
	m_pImmediateContext->setObjModel(obj);

	//设置着色器
	m_pImmediateContext->setShader(m_pShader);

	return true;
}

void Demo::Update(double dt)
{

	double x = m_radius * sinf(m_phi) * cosf(m_theta);
	double z = m_radius * sinf(m_phi) * sinf(m_theta);
	double y = m_radius * cosf(m_phi);

	Vec4 pos(x, y, z, 1.0);
	Vec4 lookAt(0.0, 0.0, 0.0, 1.0);
	Vec4 up(0.0, 1.0, 0.0, 0.0);

	Camera camera(pos, lookAt, up);

	Mat4 view = getViewMatrix(camera);
	Mat4 proj = getProjectionMatrix(PI / 4, m_pDevice->getClientWidth() /
		static_cast<double>(m_pDevice->getClientHeight()), 1.0, 100.0);

	Mat4 world(0.005);
	m_worldViewProj = world * view*proj;
	m_world = world;

	//计算逆矩阵的转置
	m_worldInvTranspose = transposeMatrix(inverseMatrix(world));

	m_pShader->setCamera(camera);
}

void Demo::Render()
{
	//清空后缓冲图片
	m_pDevice->clearBuffer(Vec4(0.0, 0.0, 0.0, 1.0));

	//设置渲染状态
	m_pImmediateContext->setRenderMode(SOLID);
	//设置着色器变量
	m_pShader->setWorldViewProj(m_worldViewProj);
	m_pShader->setWorld(m_world);
	m_pShader->setWorldInvTranspose(m_worldInvTranspose);

	m_pImmediateContext->drawIndexed();
}

void Demo::Clear()
{
	if (m_pDevice)
		delete m_pDevice;
	if (m_pImmediateContext)
		delete m_pImmediateContext;
	if (m_pShader)
		delete m_pShader;
}

void Demo::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
	SetCapture(m_hWnd);
}

void Demo::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Demo::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		//角度转弧度
		double dx = degreeToRadian(0.25f*static_cast<double>(x - m_lastMousePos.x));
		double dy = degreeToRadian(0.25f*static_cast<double>(y - m_lastMousePos.y));

		m_theta -= dx;
		m_phi += dy;

		m_phi = clamp(m_phi, 0.1f, PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		double dx = 0.2f*static_cast<double>(x - m_lastMousePos.x);
		double dy = 0.2f*static_cast<double>(y - m_lastMousePos.y);

		m_radius += dx - dy;

		m_radius = clamp(m_radius, 3.0f, 300.0f);
	}

	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}

void Demo::OnWheelScroll(WPARAM p)
{
	
}