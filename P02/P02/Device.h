#pragma once
#include "Vec4.h"

#include <windows.h>
#include <array>

class Device
{
public:
	Device();
	Device(int width, int height);
	~Device();
	
	void DrawPixel(int x, int y, Vec4 color);
	double getZ(int x, int y) const;
	void setZ(int x, int y, double z);
	double getDepth(int x, int y) const;
	void setDepth(int x, int y, double z);
	inline double** getDepthMap() { return m_depthMap; }
	inline UINT*& getFrameBuffer() { return m_framebuffer; }
	inline int getClientWidth() { return m_width; }
	inline int getClientHeight() { return m_height; }
	void clearBuffer(Vec4 color);


private:
	int m_width;
	int m_height;
	UINT* m_framebuffer;
	double **m_zBuffer;
	double** m_depthMap;	//”√”⁄º∆À„shadow
	//std::array<std::array<Vec4, 120>, 90> debug_helper;
};