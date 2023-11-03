#include <vector>
#include "Demo.h"
#include "Shader.h"
#include "Util.h"
#include "ObjReader.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "TextureReader.h"
#include "Texture.h"

#include <fstream>
#include <sstream>
#include <string>
#include <timeapi.h>

#pragma comment( lib,"winmm.lib" )

using namespace Util;

Demo::Demo() :m_theta(0), m_phi(1)
{
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;

	m_pos = Vec4(0, 8, -8, 1.0);
	m_lookAt = Vec4(0.0, 0.0, 0.0, 1.0);
	m_zoomSpeed = 1.0;
	m_fov = PI / 4;

	//m_radius = (m_lookAt - m_pos).length();
	m_directionalLight = std::make_shared<Directional>(Vec4(0.0, -1.0, 1.0, 0.0));
	m_pointLight = std::make_shared<PointLight>(Vec4(3, 1, 0, 1.0));
	m_lightPos = m_directionalLight->direction * (-50.0);
	m_lightPos.w = 1.0;
	m_lightMoving = false;
	m_cullSwitchOn = true;

	rParameterX = 0.0;
	rParameterZ = 0.0;

	m_lightTypeSwitch = true;
}


Demo::~Demo()
{
	if (m_device)
		delete m_device;
	if (m_devicecontext)
		delete m_devicecontext;
	if (m_shader)
		delete m_shader;
}

bool Demo::init(HINSTANCE hInstance, HWND hWnd)
{
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	m_device = new Device(m_width, m_height);
	m_devicecontext = new DeviceContext();
	m_devicecontext->init(m_device);
	m_shader = new Shader();

	// 读取model文件
	//std::string objFilePath = "../ms.obj";
	ObjReader* or1 = new ObjReader;
	or1->filePath = std::string("../ms.obj");
	or1->readObjectFile();
	// 读取贴图
	TextureReader* tr1 = new TextureReader;
	std::wstring textureFilePath(L"../msTex.bmp");
	tr1->textureToColorArray(textureFilePath);
	textureFilePath = L"../msNormal.bmp";
	tr1->textureToNormalArray(textureFilePath);

	ObjReader* or2 = new ObjReader;
	or2->filePath = std::string("../floor.obj");
	or2->readObjectFile();
	// 读取贴图
	TextureReader* tr2 = new TextureReader;
	textureFilePath = L"../stoneTex.bmp";
	tr2->textureToColorArray(textureFilePath);
	textureFilePath = L"../stoneNormal.bmp";
	tr2->textureToNormalArray(textureFilePath);

	m_devicecontext->setShader(m_shader);

	/*=================================================================================*/
	m_scene = new Scene;
	Object* obj1 = new Object;
	obj1->obj = or1;
	obj1->tex = tr1;
	Mat4 wm1 = Mat4(1.0);
	wm1.m[1][3] = -1.0;
	obj1->modelMatrix = wm1;
	obj1->isClockwise = true;

	Object* obj2 = new Object;
	obj2->obj = or2;
	obj2->tex = tr2;
	Mat4 wm2 = Mat4(1.0);
	wm2.m[1][3] = -2.0;
	wm2.m[0][0] *= 3;
	wm2.m[2][2] *= 3;
	obj2->modelMatrix = wm2;
	obj2->isClockwise = true;
	m_scene->objects.push_back(obj1);
	m_scene->objects.push_back(obj2);

	m_devicecontext->setScene(m_scene);
	/*=================================================================================*/

	//设置光
	std::shared_ptr<Light> ambient = std::make_shared<Ambient>(0.4);
	
	m_shader->setLight(ambient);
	m_shader->setLight(m_directionalLight);

	//初始化点光源参数及其模型位置
	ObjReader* or3 = new ObjReader;
	or3->filePath = std::string("../icosphere.obj");
	or3->readObjectFile();
	Object* lightObject = new Object(true);
	lightObject->obj = or3;
	lightObject->tex = nullptr;
	lightObject->modelMatrix = Mat4(1) * getTranslationMatrix(3, 1, 0);
	lightObject->isClockwise = true;
	m_pointLight->setModel(lightObject);
	
	m_shader->setLight(m_pointLight);
	m_scene->objects.push_back(lightObject);

	m_devicecontext->setLightType(&m_lightTypeSwitch);

	//材质
	Material material;
	m_shader->setMaterial(material);
	m_shader->m_directionalLightPos = &m_lightPos;

	return true;
}

void Demo::update()
{
	Vec4 up(0.0, 1.0, 0.0, 0.0);

	Vec4 moveDir = (m_lookAt - m_pos).normalize();
	Vec4 moveRight = moveDir.cross(up).normalize();
	Vec4 newPos = m_pos;
	if (bUp)
	{
		newPos = m_pos + moveDir*0.1;
		//m_lookAt = m_lookAt + moveDir;
		bUp = false;
	}
	if (bDown)
	{
		newPos = m_pos - moveDir*0.1;
		//m_lookAt = m_lookAt - moveDir;
		bDown = false;
	}
	if (bLeft)
	{
		newPos = m_pos + moveRight *0.1;
		m_lookAt = m_lookAt + moveRight *0.1;
		bLeft = false;
	}
	if (bRight)
	{
		newPos = m_pos - moveRight *0.1;
		m_lookAt = m_lookAt - moveRight *0.1;
		bRight = false;
	}
	Mat4 rotateMatrixY(cos(0.01), 0, -sin(0.01), 0,
		0, 1, 0, 0,
		sin(0.01), 0, cos(0.01), 0,
		0, 0, 0, 1);
	
	Mat4 rotateMatrixZ(cos(rParameterZ), -sin(rParameterZ), 0, 0,
		sin(rParameterZ), cos(rParameterZ), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	Mat4 rotateMatrixX(1.0, 0.0, 0.0, 0.0,
		0.0, cos(rParameterX), sin(rParameterX), 0.0,
		0.0, -sin(rParameterX), cos(rParameterX), 0.0,
		0.0, 0.0, 0.0, 1.0);
	if (m_lightTypeSwitch == true)
	{
		m_pointLight->position = getTranslationMatrix(rParameterX, 0, rParameterZ)*m_pointLight->position;
		m_pointLight->lightModel->modelMatrix = m_pointLight->lightModel->modelMatrix * getTranslationMatrix(rParameterX, 0, rParameterZ);
		rParameterX = rParameterZ = 0.0;
	}
	else
	{
		if (m_lightMoving)
		{
			m_directionalLight->direction = rotateMatrixY * m_directionalLight->direction;
			m_lightPos = rotateMatrixY * m_lightPos;
		}
		else
		{
			m_directionalLight->direction = rotateMatrixZ * rotateMatrixX * m_directionalLight->direction;
			m_lightPos = rotateMatrixZ * rotateMatrixX * m_lightPos;
			rParameterX = rParameterZ = 0.0;
		}
	}
	

	

	m_pos = newPos;
	m_pos = Vec4(m_pos.length()* sin(m_phi) * cos(m_theta), m_pos.length()* cos(m_phi), m_pos.length()* sin(m_phi) * sin(m_theta));

	Camera camera(m_pos, m_lookAt, up);

	Mat4 view = getViewMatrix(camera);
	Mat4 proj = getPerspectiveProjectionMatrix(m_fov, m_device->getClientWidth() / static_cast<double>(m_device->getClientHeight()), 5, 20);
	m_viewMatrix = view;
	m_projMatrix = proj;
	/*
	Mat4 rotation(1.0);
	rotation.m[0][0] = cos(90);
	rotation.m[0][1] = -sin(90);
	rotation.m[1][0] = sin(90);
	rotation.m[1][1] = cos(90);
	*/

	m_shader->setCamera(camera);
}

void Demo::Render()
{
	//清空后缓冲图片
	m_device->clearBuffer(Vec4(0.2, 0.2, 0.2, 1.0));
	clearDrawCalls();
	//m_device->clearBuffer(Vec4(0.0, 0.5, 0.5, 1.0));

	m_devicecontext->setRenderMode(RM);				//设置渲染模式
	m_shader->setViewMatrix(m_viewMatrix);			
	m_shader->setProjectionMatrix(m_projMatrix);

	m_devicecontext->drawIndexed(m_drawCalls, m_cullSwitchOn);
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
		double dx = degreeToRadian(0.25*static_cast<double>(x - static_cast<int>(m_lastMousePos.x)));
		double dy = degreeToRadian(0.25*static_cast<double>(y - static_cast<int>(m_lastMousePos.y)));

		m_theta -= dx;
		m_phi -= dy;

		m_phi = clamp(m_phi, 0.1, PI - 0.1);
	}
	else if ((btnState&MK_RBUTTON) != 0)
	{
		double dx = degreeToRadian(0.25*static_cast<double>(x - static_cast<int>(m_lastMousePos.x)));
		double dy = degreeToRadian(0.25*static_cast<double>(y - static_cast<int>(m_lastMousePos.y)));
		rParameterZ = dx;
		rParameterX = dy;
	}
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}

void Demo::onWheelScroll(int p)
{
	if (p > 0 && m_fov > 0.1)
	{
		m_fov -= 0.05;

	}
	else if (p < 0 && m_fov < PI/2)
	{
		m_fov += 0.05;
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
		RM = RENDER_MODE::COLOR;
	}
	else if (f == 3)
	{
		RM = RENDER_MODE::TEXTURE_NO_LIGHT;
	}
	else if (f == 4)
	{
		RM = RENDER_MODE::TEXTURE_WITH_LIGHT;
	}
}

void Demo::modelSwitch()
{	
	ObjReader* curObj = m_devicecontext->getObjReader();
	if (curObj->filePath == std::string("../mech.obj"))
	{

		ObjReader* or1 = new ObjReader;
		or1->filePath = std::string("../brick.obj");
		or1->readObjectFile();

		TextureReader *tr1 = new TextureReader;
		std::wstring textureFilePath(L"../soilTex.bmp");
		tr1->textureToColorArray(textureFilePath);
		textureFilePath = L"../soilNormal.bmp";
		tr1->textureToNormalArray(textureFilePath);
		Object* obj1 = new Object;
		obj1->obj = or1;
		obj1->tex = tr1;
		Mat4 wm1 = Mat4(1.0);
		wm1.m[1][3] = -1.0;
		obj1->modelMatrix = wm1;
		obj1->isClockwise = true;
		delete m_scene->objects[0];
		m_scene->objects[0] = obj1;
	}
	else
	{
		ObjReader* or1 = new ObjReader;
		or1->filePath = std::string("../mech.obj");
		or1->readObjectFile();

		TextureReader *tr1 = new TextureReader;
		std::wstring textureFilePath(L"../mechTex.bmp");
		tr1->textureToColorArray(textureFilePath);
		textureFilePath = L"../mechNormal.bmp";
		tr1->textureToNormalArray(textureFilePath);
		Object* obj1 = new Object;
		obj1->obj = or1;
		obj1->tex = tr1;
		Mat4 wm1 = Mat4(0.3);
		wm1.m[1][3] = -2;
		obj1->modelMatrix = wm1;
		obj1->isClockwise = true;
		delete m_scene->objects[0];
		m_scene->objects[0] = obj1;

	}
	
}

void Demo::lightMovingSwitch()
{
	m_lightMoving = !m_lightMoving;
}

void Demo::toggleCullSwitch()
{
	m_cullSwitchOn = !m_cullSwitchOn;
}