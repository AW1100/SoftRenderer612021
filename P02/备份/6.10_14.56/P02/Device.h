#pragma once
#include <windows.h>
#include "Vec4.h"

class Device
{
public:
	Device(int width, int height);
	~Device();
	
	void DrawPixel(int x, int y, Vec4 color);
	double getZ(int x, int y) const;
	void setZ(int x, int y, double z);
	inline UINT*& getFrameBuffer() { return m_pFramebuffer; }
	inline int getClientWidth() { return m_width; }
	inline int getClientHeight() { return m_height; }
	void clearBuffer(Vec4 color);

private:
	int m_width;
	int m_height;
	UINT* m_pFramebuffer;
	double **m_zBuffer;
};