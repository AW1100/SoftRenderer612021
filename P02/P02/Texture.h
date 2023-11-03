#pragma once
#include "Vec4.h"
#include "Vec2.h"
#include <memory>

class Texture
{
public:
	Texture() = default;
	Texture(unsigned int width, unsigned int height);
	~Texture();
	Texture& operator = (const Texture& rhs);

	Vec2 sample(const Vec2& tex);

	unsigned int m_width;
	unsigned int m_height;
	Vec4** m_texMap;
};