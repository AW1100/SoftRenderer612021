#pragma once
#include "ObjReader.h"
#include "TextureReader.h"
#include "Mat4.h"
#include "Util.h"

#include <vector>

struct Object
{
	Object()
	{
		isSpecialObject = false;
	}

	Object(bool iso_) :
		isSpecialObject(iso_)
	{}

	~Object()
	{
		if (obj != nullptr)
			delete obj;
		if (tex != nullptr)
			delete tex;
	}
	inline Mat4& getModelMatrix() { return modelMatrix; }
		
	ObjReader* obj;
	TextureReader* tex;
	Mat4 modelMatrix;
	bool isClockwise;
	bool isSpecialObject;
};

struct Scene
{
	Scene() = default;
	
	std::vector<Object*> objects;
};