#include "Device.h"
#include "Util.h"
using namespace Util;

Device::Device(int width, int height)
{
	m_width = width;
	m_height = height;
	m_zBuffer = new double*[width];
	for (int i = 0; i < width; i++)
	{
		m_zBuffer[i] = new double[height];
	}
}

Device::~Device()
{
	if (m_pFramebuffer)
		delete m_pFramebuffer;
	if (m_zBuffer)
	{
		for (int i = 0; i < m_width; i++)
		{
			delete[] m_zBuffer[i];
		}
	}
}

void Device::DrawPixel(int x, int y, Vec4 color)
{
	m_pFramebuffer[m_width*y + x] = colorToUINT(color);
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

void Device::clearBuffer(Vec4 color)
{
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			m_pFramebuffer[m_width*y + x] = colorToUINT(color);
			m_zBuffer[x][y] = 0;
		}
	}
}