#pragma once
#include "Device.h"
#include "Shader.h"
#include <vector>
#include "Vec4.h"
#include "ObjReader.h"
#include "Util.h"
#include "TextureReader.h"

static RENDER_MODE RM = RENDER_MODE::SOLID_NO_LIGHT;

class DeviceContext
{
public:
	DeviceContext();
	~DeviceContext();

	void init(Device* pDevice);
	void setRenderMode(RENDER_MODE mode);
	void setShader(Shader* s);
	void drawIndexed();
	inline void setObjModel(ObjReader* o) { m_obj = o; }
	inline void setTexture(TextureReader t) { m_texture = t; }
	//inline void setRenderMode() { m_renderMode = RENDER_MODE::WIREFRAME; }

private:
	void toCVV(VertexOut& v);
	bool clip(VertexOut& v);
	VertexOut transformToProj(const VertexIn& v);
	void transformToScreen(const Mat4& m, VertexOut& v);
	bool backFaceCulling(const VertexIn& v1, const VertexIn& v2, const VertexIn& v3);
	void drawLine(int x1, int y1, int x2, int y2);
	void scanLineFill(const VertexOut& left, const VertexOut& right, int yIndex);
	void drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);

	Device* m_device;
	RENDER_MODE m_renderMode;
	ObjReader* m_obj;
	Shader* m_shader;
	TextureReader m_texture;
};