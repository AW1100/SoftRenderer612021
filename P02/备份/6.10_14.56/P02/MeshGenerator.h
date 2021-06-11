#pragma once
#include "Vertex.h"
#include <vector>

class MeshGenerator
{
private:
	MeshGenerator() {}

public:
	static MeshGenerator* getInstance()
	{
		static MeshGenerator instance;
		return &instance;
	}

	struct MeshData
	{
		std::vector<VertexIn> vertices;
		std::vector<int> indices;
	};

	void createMesh(MeshData &mesh);
};