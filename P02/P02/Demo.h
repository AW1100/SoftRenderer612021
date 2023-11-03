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
#include "Texture.h"
#include "Scene.h"

class Demo
{
public:
	Demo();
	~Demo();
public:
	bool init(HINSTANCE hInstance, HWND hWnd);
	void update();
	void Render();

	//鼠标事件控制
	void onMouseDown(WPARAM btnState, int x, int y);
	void onMouseUp(WPARAM btnState, int x, int y);
	void onMouseMove(WPARAM btnState, int x, int y);
	void onWheelScroll(int p);
	void onKeyPressed(int f);
	void modelSwitch();
	void lightMovingSwitch();
	void toggleCullSwitch();
	inline bool getCullSwitch() { return m_cullSwitchOn; }
	inline int getDrawCalls() const { return m_drawCalls; }
	inline void clearDrawCalls() { m_drawCalls = 0; }
	inline void upKeyPressed() { bUp = true; }
	inline void downKeyPressed() { bDown = true; }
	inline void leftKeyPressed() { bLeft = true; }
	inline void rightKeyPressed() { bRight = true; }
	inline Device*& GetDevice() { return m_device; }

private:
	int					m_width, m_height;
	HINSTANCE			m_hInstance;
	HWND				m_hWnd;

	Device*				m_device;
	DeviceContext*		m_devicecontext;
	Shader*				m_shader;
	Scene*				m_scene;

	Mat4				m_viewMatrix;
	Mat4				m_projMatrix;

	//控制摄像机位置角度等
	double				m_theta;
	double				m_phi;
	double				m_radius;
	POINT				m_lastMousePos;
	Vec4				m_pos;
	Vec4				m_lookAt;
	double				m_zoomSpeed;
	double				m_fov;

	std::shared_ptr<Directional>	m_directionalLight;
	Vec4							m_lightPos;
	bool							m_lightMoving;

	std::shared_ptr<PointLight>		m_pointLight;

	double				rParameterZ;
	double				rParameterX;
	int					m_drawCalls;
	bool				m_cullSwitchOn;

	bool				bUp;
	bool				bDown;
	bool				bLeft;
	bool				bRight;

	bool				m_lightTypeSwitch;
};

