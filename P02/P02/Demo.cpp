#include <vector>
#include "Demo.h"
#include "Shader.h"
#include "Util.h"
#include "ObjReader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "TextureReader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <timeapi.h>

#pragma comment( lib,"winmm.lib" )

using namespace Util;

Demo::Demo() :m_theta(0.75 * PI), m_phi(0.5*PI), m_radius(4.0)
{
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;

	m_world = Mat4(1.0);

	m_pos = Vec4(m_radius * sin(m_phi) * cos(m_theta), m_radius * sin(m_phi) * sin(m_theta), m_radius * cos(m_phi), 1.0);
	m_lookAt = Vec4(0.0, 2.0, 0.0, 1.0);
	m_zoomSpeed = 1.0;
}


Demo::~Demo()
{
	if (m_pDevice)
		delete m_pDevice;
	if (m_pImmediateContext)
		delete m_pImmediateContext;
	if (m_pShader)
		delete m_pShader;
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
	std::string filePath = "../ms.obj";
	ObjReader* obj = new ObjReader;
	obj->readObjectFile(filePath);
	int count = static_cast<int>(obj->uv.size());

	TextureReader tr;
	tr.textureToColorArray();

	m_pImmediateContext->setObjModel(obj);
	m_pImmediateContext->setTexture(tr);
	//设置着色器
	m_pImmediateContext->setShader(m_pShader);
	std::shared_ptr<Light> ambient = std::make_shared<Ambient>(0.05);
	std::shared_ptr<Light> directional = std::make_shared<Directional>(0.5, Vec4(0.0,0.0,-1.0));
	m_pShader->setLight(ambient);
	m_pShader->setLight(directional);
	Material material;
	m_pShader->setMaterial(material);

	return true;
}

void Demo::update(double dt)
{

	double x = m_radius * sin(m_phi) * cos(m_theta) * m_zoomSpeed;
	double z = m_radius * sin(m_phi) * sin(m_theta) * m_zoomSpeed;
	double y = m_radius * cos(m_phi) * m_zoomSpeed;
	m_radius *= m_zoomSpeed;
	m_zoomSpeed = 1.0;
	m_pos =Vec4(x, y, z, 1.0);
	m_lookAt = Vec4(0.0, 2.0, 0.0, 1.0);
	Vec4 up(0.0, 1.0, 0.0, 0.0);
	
	Camera camera(m_pos, m_lookAt, up);

	Mat4 view = getViewMatrix(camera);
	Mat4 proj = getProjectionMatrix(PI / 4, m_pDevice->getClientWidth() /
		static_cast<double>(m_pDevice->getClientHeight()), 1.0, 100.0);

	Mat4 world(0.3);
	Mat4 translate(1.0);
	translate.m[3][1] = 0.6;
	Mat4 rotation(1.0);
	rotation.m[0][0] = cos(90);
	rotation.m[0][1] = -sin(90);
	rotation.m[1][0] = sin(90);
	rotation.m[1][1] = cos(90);
	world = world * translate;
	m_world = world;
	m_worldViewProj = m_world * view*proj;

	//计算逆矩阵的转置
	m_worldInvTranspose = transposeMatrix(inverseMatrix(world));

	m_pShader->setCamera(camera);
}

void Demo::Render()
{
	//清空后缓冲图片
	m_pDevice->clearBuffer(Vec4(0.1, 0.1, 0.1, 1.0));

	//设置渲染状态
	m_pImmediateContext->setRenderMode(RM);
	//设置着色器变量
	m_pShader->setWorldViewProj(m_worldViewProj);
	m_pShader->setWorld(m_world);
	m_pShader->setWorldInvTranspose(m_worldInvTranspose);

	m_pImmediateContext->drawIndexed();
	
}



void Demo::onMouseDown(WPARAM btnState, int x, int y)
{
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
	SetCapture(m_hWnd);
}

void Demo::onMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Demo::onMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		//角度转弧度
		double dx = degreeToRadian(0.25*static_cast<double>(x - m_lastMousePos.x));
		double dy = degreeToRadian(0.25*static_cast<double>(y - m_lastMousePos.y));

		m_theta -= dx;
		m_phi += dy;

		m_phi = clamp(m_phi, 0.1, PI - 0.1);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		double dx = 0.2*static_cast<double>(x - m_lastMousePos.x);
		double dy = 0.2*static_cast<double>(y - m_lastMousePos.y);

		m_radius += dx - dy;

		m_radius = clamp(m_radius, 3.0, 300.0);
	}

	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}

void Demo::onWheelScroll(int p)
{
	if (p > 0)
	{
		m_zoomSpeed = 0.9;
	}
	else
	{
		m_zoomSpeed = 1.1;
	}
}

void Demo::onKeyPressed(int f)
{
	if (f == 1)
	{
		RM = RENDER_MODE::WIREFRAME;
	}
	else if (f == 2)
	{
		RM = RENDER_MODE::SOLID_NO_LIGHT;
	}
	else if (f == 3)
	{
		RM = RENDER_MODE::SOLID_WITH_LIGHT;
	}
	//m_pDevice->clearBuffer(Vec4(0.1, 0.1, 0.1, 1.0));
}