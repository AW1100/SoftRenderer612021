#include "Device.h"
#include "Util.h"
using namespace Util;

Device::Device()
{
	m_width = 0;
	m_height = 0;
	m_framebuffer = nullptr;
	m_zBuffer = nullptr;
	m_depthMap = nullptr;
}

Device::Device(int width, int height)
{
	m_width = width;
	m_height = height;
	m_framebuffer = nullptr;
	m_zBuffer = new double*[width];
	for (int i = 0; i < width; i++)
	{
		m_zBuffer[i] = new double[height];
	}
	m_depthMap = new double* [width];
	for (int i = 0; i < width; i++)
	{
		m_depthMap[i] = new double[height];
	}
}

Device::~Device()
{

}

void Device::DrawPixel(int x, int y, Vec4 color)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		/*
		if (x > 70 && x < 130 && y>50 && y < 100 && color == Vec4(0.0,1.0,1.0,1.0))
		{
			int i = 0;
		}
		*/
		//debug_helper[y][x] = color;
		m_framebuffer[m_width*y + x] = colorToUINT(color);
	}
}

double Device::getZ(int x, int y) const
{
	if (x >= 0 && x < m_width&&y >= 0 && y < m_height)
		return m_zBuffer[x][y];
	else
		return 1.0;
}

void Device::setZ(int x, int y, double z)
{
	if (x >= 0 && x < m_width&&y >= 0 && y < m_height)
	{
		m_zBuffer[x][y] = z;
	}
}

double Device::getDepth(int x, int y) const
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		return m_depthMap[x][y];
	else
		return 1.0;
}

void Device::setDepth(int x, int y, double z)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		m_depthMap[x][y] = z;
	}
}


void Device::clearBuffer(Vec4 color)
{	
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			m_framebuffer[m_width*y + x] = colorToUINT(color);
			m_zBuffer[x][y] = 0;
			m_depthMap[x][y] = 0;
		}
	}	
}