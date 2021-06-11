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

	std::vector<VertexIn> vertices;
	std::vector<int> indices;
};