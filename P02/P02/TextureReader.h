#pragma once
#include "Vec4.h"
#include "Vec2.h"

#include <vector>

class TextureReader
{
public:
	TextureReader()
	{
		colorArray.reserve(4194304);
	}
	void textureToColorArray();


	std::vector<Vec4> colorArray;
};