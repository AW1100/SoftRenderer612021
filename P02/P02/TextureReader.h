#pragma once
#include "Vec4.h"
#include "Vec2.h"
#include "Texture.h"

#include <vector>
#include <string>

class TextureReader
{
public:
	TextureReader();
	~TextureReader()
	{
		if (texture_buffer != nullptr)
			delete texture_buffer;
		if (normal_buffer != nullptr)
			delete normal_buffer;
	}
	
	void textureToColorArray(std::wstring infilename);
	void textureToNormalArray(std::wstring infilename);


	Texture* texture_buffer;
	Texture* normal_buffer;
};