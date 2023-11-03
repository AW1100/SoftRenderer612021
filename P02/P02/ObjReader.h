#pragma once
#include <vector>
#include <Vertex.h>
#include <fstream>
#include <string>
#include <sstream>


class ObjReader
{
public:
	ObjReader() = default;
	~ObjReader() = default;
	void readObjectFile();

	std::string filePath;
	std::vector<Vec4> pos;
	std::vector<Vec4> normal;
	std::vector<Vec2> uv;
	std::vector<int> indices;
};