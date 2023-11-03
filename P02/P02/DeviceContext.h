#pragma once
#include "Device.h"
#include "Shader.h"
#include "Vec4.h"
#include "ObjReader.h"
#include "Util.h"
#include "TextureReader.h"
#include "Scene.h"

#include <vector>

static RENDER_MODE RM = RENDER_MODE::TEXTURE_WITH_LIGHT;

class DeviceContext
{
public:
	DeviceContext();
	~DeviceContext();

	void init(Device* pDevice);
	void setRenderMode(RENDER_MODE mode);
	void setShader(Shader* s);
	void drawIndexed(int& trisNum, bool& cullSwitch);
	//inline void setObjModel(ObjReader* o) { m_obj = o; }
	inline void setTexture(TextureReader* t) { m_texture = t; }
	ObjReader* getObjReader();
	inline void setScene(Scene* scene) { m_scene = scene; }
	inline void setLightType(bool* lts) { m_lightTypeSwitch = lts; }

private:
	void toNDC(Vec4& v);
	bool clip(VertexOut& v1, VertexOut& v2, VertexOut& v3, int &trisNum);
	VertexOut transformToProj(const VertexIn& v);
	void transformToScreen(Vec4& v);
	bool backFaceCulling(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3, bool isClockwise);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void shadowTriangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);

	Vec4 barycentric(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec4 &P);
	void drawScanLine(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void shadowdrawScanLine(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);

	VertexOut shadowTransformToProj(const VertexIn& v);

	void clipWithOneVertexOutside(VertexOut& v1, VertexOut& v2, VertexOut& v3, int& trisNum);
	void clipWithTwoVertexOutside(VertexOut& v1, VertexOut& v2, VertexOut& v3, int& trisNum);
	void postProcessTriangle(VertexOut v1, VertexOut v2, VertexOut v3, int& trisNum);

	void perspectiveCorrection(VertexOut& out);
	inline void setPointLightPosition(const Vec4& pos) { m_pointLightPos = pos; }
	

private:
	Device* m_device;
	RENDER_MODE m_renderMode;
	Shader* m_shader;
	TextureReader* m_texture;
	Scene* m_scene;
	bool m_curObjectState;
	bool* m_lightTypeSwitch;

	Vec4 m_pointLightPos;

	//double debugV1;
};