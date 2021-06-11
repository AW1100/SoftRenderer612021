#pragma once
#include "Device.h"
#include "ShaderBase.h"
#include <vector>
#include "Vec4.h"

enum RENDER_MODE { WIREFRAME, SOLID };

class DeviceContext
{
public:
	DeviceContext();
	~DeviceContext();

	void init(Device* pDevice);
	void setRenderMode(RENDER_MODE mode);
	void setVertexBuffer(std::vector<VertexIn> vertices);
	void setCameraPos(const Vec4& pos);
	void setIndexBuffer(std::vector<UINT> indices);
	void setShader(ShaderBase* base);
	void drawIndexed(UINT indexCount, UINT startIndexLoc, UINT startVertexLoc);

private:
	void toCVV(VertexOut& v);
	bool clip(const VertexOut& v);
	VertexOut transformToProj(const VertexIn& v);
	void transformToScreen(const Mat4& m, VertexOut& v);
	bool backFaceCulling(const VertexIn& v1, const VertexIn& v2, const VertexIn& v3);
	void drawLine(int x1, int y1, int x2, int y2);
	void scanLineFill(const VertexOut& left, const VertexOut& right, int yIndex);
	void drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);

	Device* m_pDevice;
	RENDER_MODE m_renderMode;
	std::vector<VertexIn> m_vertices;
	std::vector<UINT> m_indices;
	ShaderBase* m_pShader;
	Vec4 m_cameraPos;
};