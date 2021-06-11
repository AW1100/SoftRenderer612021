#include "MeshGenerator.h"

void MeshGenerator::createMesh(MeshData &mesh)
{
	mesh.vertices.clear();
	mesh.indices.clear();

	mesh.vertices.resize(4);
	mesh.indices.resize(6);

	mesh.vertices[0].pos = Vec4(-0.5, -0.5, -0.5, 1.0);
	mesh.vertices[0].normal = Vec4(0.0, 0.0, -1.0);
	mesh.vertices[0].color = Vec4(1.0, 0.0, 0.0, 1.0);

	mesh.vertices[1].pos = Vec4(-0.5, 0.5, -0.5, 1.0);
	mesh.vertices[1].normal = Vec4(0.0, 0.0, -1.0);
	mesh.vertices[1].color = Vec4(0.0, 0.0, 0.0, 1.0);

	mesh.vertices[2].pos = Vec4(0.5, 0.5, -0.5, 1.0);
	mesh.vertices[2].normal = Vec4(0.0, 0.0, -1.0);
	mesh.vertices[2].color = Vec4(1.0, 0.0, 0.0, 1.0);

	mesh.vertices[3].pos = Vec4(0.5, -0.5, -0.5, 1.0);
	mesh.vertices[3].normal = Vec4(0.0, 0.0, -1.0);
	mesh.vertices[3].color = Vec4(0.0, 1.0, 0.0, 1.0);

	mesh.indices[0] = 0;
	mesh.indices[1] = 1;
	mesh.indices[2] = 2;
	mesh.indices[3] = 0;
	mesh.indices[4] = 2;
	mesh.indices[5] = 3;

}