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
	void readObjectFile(std::string filePath);

	//std::vector<VertexIn> vertices;
	std::vector<Vec4> pos;
	std::vector<Vec4> normal;
	std::vector<unsigned int> indices;
};