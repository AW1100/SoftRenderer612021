#pragma once
#include "Vec4.h"
#include <memory>

class Texture
{
public:
	Texture();


	unsigned int m_width;
	unsigned int m_height;
	std::shared_ptr<std::shared_ptr<Vec4>> m_texMap;
};