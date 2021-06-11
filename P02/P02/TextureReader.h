#pragma once
#include "Vec4.h"
#include "Vec2.h"

#include <vector>

class TextureReader
{
public:
	void textureToColorArray();


	std::vector<Vec4> colorArray;
};