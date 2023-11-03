#include "Texture.h"

Texture::Texture(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_texMap = new Vec4*[width];
	for (unsigned int i = 0; i < width; i++)
	{
		m_texMap[i] = new Vec4[height];
	}

	
}

Texture::~Texture()
{
	if (m_texMap)
	{
		for (unsigned int i = 0; i < m_width; i++)
		{
			delete[] m_texMap[i];
		}
	}
}

Texture& Texture::operator=(const Texture& rhs)
{
	if (this == &rhs)
		return *this;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_texMap = new Vec4*[m_width];
	for (unsigned int i = 0; i < m_width; i++)
	{
		m_texMap[i] = new Vec4[m_height];
		for (unsigned int j = 0; j < m_height; j++)
		{
			m_texMap[i][j] = rhs.m_texMap[i][j];
		}
	}
	return *this;
}

Vec2 Texture::sample(const Vec2& tex)
{
	if (tex.x >= 0 && tex.x <= 1 && tex.y >= 0 && tex.y <= 1)
	{
		unsigned int x = static_cast<unsigned int>(tex.x*(m_width));
		unsigned int y = static_cast<unsigned int>(tex.y*(m_height));
		return Vec2(x, y);
	}
	else
	{

		double u, v;
		u = 0;
		v = 0;
		if (tex.x > 1)
		{
			u = tex.x - static_cast<int>(tex.x);
		}
		else if (tex.x < 0)
		{
			u = (static_cast<int>(-tex.x) + 1.0) + tex.x;
		}
		if (tex.y > 1)
		{
			v = tex.y - static_cast<int>(tex.y);
		}
		else if (tex.y < 0)
		{
			v = (static_cast<int>(-tex.y) + 1.0) + tex.y;
		}
		unsigned int x = static_cast<unsigned int>(u * (m_width - 1));
		unsigned int y = static_cast<unsigned int>(v * (m_height - 1));
		return Vec2(x,y);
	}
}