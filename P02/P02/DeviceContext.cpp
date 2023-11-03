#include "DeviceContext.h"
#include "Vertex.h"

#include <algorithm>
#include <vector>

using namespace Util;

DeviceContext::DeviceContext() :m_renderMode(RENDER_MODE::WIREFRAME)
{
	m_device = nullptr;
	m_scene = nullptr;
	m_shader = nullptr;
	m_texture = nullptr;
	m_lightTypeSwitch = nullptr;
}


DeviceContext::~DeviceContext()
{

}

void DeviceContext::init(Device* pDevice)
{
	m_device = pDevice;
}

//设置渲染模式
void DeviceContext::setRenderMode(RENDER_MODE mode)
{
	m_renderMode = mode;
}

//设置着色器
void DeviceContext::setShader(Shader* s)
{
	m_shader = s;
}

ObjReader* DeviceContext::getObjReader()
{
	if (m_scene->objects.size() > 0)
	{
		return m_scene->objects[0]->obj;
	}
	return nullptr;
}

//绘制顶点缓冲中的三角形
void DeviceContext::drawIndexed(int& trisNum, bool& cullSwitch)
{
	//Vec4 lightPos(25.0, -50.0, 25.0, 1.0);
	Vec4 lightLookAt(0.0, 0.0, 0.0, 1.0);
	Vec4 lightUp(0.0, 1.0, 0.0, 0.0);
	//Camera lc(*(m_shader->m_directionalLightPos), lightLookAt, lightUp);
	Mat4 lightViewMatrix;
	Mat4 lightProjMatrix;
	if (*m_lightTypeSwitch == true)
	{
		std::shared_ptr<PointLight> pl = std::dynamic_pointer_cast<PointLight>(m_shader->m_lights[2]);
		Camera lc(pl->position, lightLookAt, lightUp);
		lightViewMatrix = getViewMatrix(lc);	
		lightProjMatrix = getPerspectiveProjectionMatrix(PI / 4 + 0.1, m_device->getClientWidth() / static_cast<double>(m_device->getClientHeight()), 0.1, 100);
	}
	else
	{
		Camera lc(*(m_shader->m_directionalLightPos), lightLookAt, lightUp);
		Mat4 lightViewMatrix = getViewMatrix(lc);
		lightProjMatrix = getOrthographicProjectionMatrix(m_device->getClientHeight() / 100, m_device->getClientWidth() / static_cast<double>(m_device->getClientHeight()), 0.1, 100);
	}	
	m_shader->m_lightSpaceMatrix = lightProjMatrix * lightViewMatrix;



	// first pass计算shadow map
	for (int n = 0; n < m_scene->objects.size(); n++)
	{
		if (m_scene->objects[n]->isSpecialObject)
			continue;
		m_shader->m_obj = m_scene->objects[n];
		m_shader->setWorld(m_scene->objects[n]->getModelMatrix());

		for (int i = 0; i < m_scene->objects[n]->obj->indices.size(); i = i + 9)
		{
			Vec4 empty4(0.0, 0.0, 0.0, 0.0);
			Vec2 empty2(0.0, 0.0);

			Vec4 pos1 = m_scene->objects[n]->obj->pos[m_scene->objects[n]->obj->indices[i] - 1];
			Vec4 pos2 = m_scene->objects[n]->obj->pos[m_scene->objects[n]->obj->indices[i + 3] - 1];
			Vec4 pos3 = m_scene->objects[n]->obj->pos[m_scene->objects[n]->obj->indices[i + 6] - 1];

			VertexIn p1(pos1, empty4, empty4, empty2, empty4, empty4);
			VertexIn p2(pos2, empty4, empty4, empty2, empty4, empty4);
			VertexIn p3(pos3, empty4, empty4, empty2, empty4, empty4);

			VertexOut v1 = shadowTransformToProj(p1);
			VertexOut v2 = shadowTransformToProj(p2);;
			VertexOut v3 = shadowTransformToProj(p3);

			toNDC(v1.fragPos);
			toNDC(v2.fragPos);
			toNDC(v3.fragPos);
			
			//将投影得到的坐标转化为屏幕坐标
			transformToScreen(v1.fragPos);
			transformToScreen(v2.fragPos);
			transformToScreen(v3.fragPos);
			

			shadowTriangleRasterization(v1, v2, v3);
		}

	}
	
	for (int n = 0; n < m_scene->objects.size(); n++)
	{
		m_curObjectState = m_scene->objects[n]->isSpecialObject;
		m_shader->m_obj = m_scene->objects[n];
		m_shader->setWorld(m_scene->objects[n]->getModelMatrix());
		if (m_curObjectState == false)
		{
			m_shader->setTexture(m_scene->objects[n]->tex->texture_buffer);
			m_shader->setNormal(m_scene->objects[n]->tex->normal_buffer);
		}
		

		for (int i = 0; i < m_scene->objects[n]->obj->indices.size(); i = i + 9)	//m_scene->objects[n]->obj->indices.size()
		{
			Vec2 uv1 = m_scene->objects[n]->obj->uv[m_scene->objects[n]->obj->indices[i + 1] - 1];
			Vec2 uv2 = m_scene->objects[n]->obj->uv[m_scene->objects[n]->obj->indices[i + 4] - 1];
			Vec2 uv3 = m_scene->objects[n]->obj->uv[m_scene->objects[n]->obj->indices[i + 7] - 1];

			Vec4 pos1 = m_scene->objects[n]->obj->pos[m_scene->objects[n]->obj->indices[i] - 1];
			Vec4 pos2 = m_scene->objects[n]->obj->pos[m_scene->objects[n]->obj->indices[i + 3] - 1];
			Vec4 pos3 = m_scene->objects[n]->obj->pos[m_scene->objects[n]->obj->indices[i + 6] - 1];

			//计算tangent、bitangent
			Vec4 edge1 = pos2 - pos1;
			Vec4 edge2 = pos3 - pos1;
			Vec2 deltaUV1 = uv2 - uv1;
			Vec2 deltaUV2 = uv3 - uv1;
			double f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			double t1 = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			double t2 = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			double t3 = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			Vec4 tangent(t1, t2, t3, 0.0);

			double b1 = f * (-deltaUV2.x * edge1.x - deltaUV1.x * edge2.x);
			double b2 = f * (-deltaUV2.x * edge1.y - deltaUV1.x * edge2.y);
			double b3 = f * (-deltaUV2.x * edge1.z - deltaUV1.x * edge2.z);
			Vec4 bitangent(b1, b2, b3, 0.0);
			//=====================
			std::vector<Vec4> colorArray;
			colorArray.push_back(Vec4(1.0, 0.0, 0.0));
			colorArray.push_back(Vec4(0.0, 1.0, 0.0));
			colorArray.push_back(Vec4(0.0, 0.0, 1.0));

			VertexIn p1(pos1, colorArray[(m_scene->objects[n]->obj->indices[i] - 1) % 3], m_scene->objects[n]->obj->normal[m_scene->objects[n]->obj->indices[i + 2] - 1], uv1, tangent, bitangent);
			VertexIn p2(pos2, colorArray[(m_scene->objects[n]->obj->indices[i + 3] - 1) % 3], m_scene->objects[n]->obj->normal[m_scene->objects[n]->obj->indices[i + 5] - 1], uv2, tangent, bitangent);
			VertexIn p3(pos3, colorArray[(m_scene->objects[n]->obj->indices[i + 6] - 1) % 3], m_scene->objects[n]->obj->normal[m_scene->objects[n]->obj->indices[i + 8] - 1], uv3, tangent, bitangent);
			
			//转换到齐次裁剪空间，即投影后的坐标
			VertexOut v1 = transformToProj(p1);
			VertexOut v2 = transformToProj(p2);
			VertexOut v3 = transformToProj(p3);

			//背面消隐
			if (backFaceCulling(v1, v2, v3, m_shader->m_obj->isClockwise) == false && cullSwitch == true)
				continue;
			
			if (clip(v1,v2,v3,trisNum))
				continue;

			perspectiveCorrection(v1);
			perspectiveCorrection(v2);
			perspectiveCorrection(v3);
			
			//进行透视除法 转到cvv
			toNDC(v1.fragPos);
			toNDC(v2.fragPos);
			toNDC(v3.fragPos);
			toNDC(v1.fragPosLightSpace);
			toNDC(v2.fragPosLightSpace);
			toNDC(v3.fragPosLightSpace);

			trisNum += 1;

			//将投影得到的坐标转化为屏幕坐标
			transformToScreen(v1.fragPos);
			transformToScreen(v2.fragPos);
			transformToScreen(v3.fragPos);
			transformToScreen(v1.fragPosLightSpace);
			transformToScreen(v2.fragPosLightSpace);
			transformToScreen(v3.fragPosLightSpace);

			drawTriangle(v1, v2, v3);

		}
	}
		
}

//转到齐次裁剪空间
VertexOut DeviceContext::transformToProj(const VertexIn& v)
{
	VertexOut out = m_shader->vertexShader(v);
	return out;
}

void DeviceContext::perspectiveCorrection(VertexOut& out)
{
	out.oneOverZ = 1.0 / out.fragPos.w;
	out.soneOverZ = 1.0 / out.fragPosLightSpace.z;

	out.color.x *= out.oneOverZ;
	out.color.y *= out.oneOverZ;
	out.color.z *= out.oneOverZ;

	out.normal.x *= out.oneOverZ;
	out.normal.y *= out.oneOverZ;
	out.normal.z *= out.oneOverZ;

	out.tex.x *= out.oneOverZ;
	out.tex.y *= out.oneOverZ;
}

void DeviceContext::toNDC(Vec4& v)
{
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w = 1.0;
}

bool DeviceContext::clip(VertexOut& v1, VertexOut& v2, VertexOut& v3, int& trisNum)
{
	if (v1.fragPos.x > v1.fragPos.w && v2.fragPos.x > v2.fragPos.w && v3.fragPos.x > v3.fragPos.w)
		return true;
	if (v1.fragPos.x < -v1.fragPos.w && v2.fragPos.x < -v2.fragPos.w && v3.fragPos.w < -v3.fragPos.w)
		return true;
	if (v1.fragPos.y > v1.fragPos.w && v2.fragPos.y > v2.fragPos.w && v3.fragPos.y > v3.fragPos.w)
		return true;
	if (v1.fragPos.y < -v1.fragPos.w && v2.fragPos.y < -v2.fragPos.w && v3.fragPos.y < -v3.fragPos.w)
		return true;
	if (v1.fragPos.z > v1.fragPos.w && v2.fragPos.z > v2.fragPos.w && v3.fragPos.z > v3.fragPos.w)
		return true;
	if (v1.fragPos.z < 0.0 && v2.fragPos.z < 0.0 && v3.fragPos.z < 0.0)
		return true;

	if (v1.fragPos.z < 0.0)
	{
		if (v2.fragPos.z < 0.0)
		{
			clipWithTwoVertexOutside(v1, v2, v3, trisNum);
			return true;
		}
		else if (v3.fragPos.z < 0.0)
		{
			clipWithTwoVertexOutside(v1, v3, v2, trisNum);
			return true;
		}
		else
		{
			clipWithOneVertexOutside(v1, v2, v3, trisNum);
			return true;
		}
	}
	else if (v2.fragPos.z < 0.0)
	{
		if (v3.fragPos.z < 0.0)
		{
			clipWithTwoVertexOutside(v2, v3, v1, trisNum);
			return true;
		}
		else
		{
			clipWithOneVertexOutside(v2, v1, v3, trisNum);
			return true;
		}
	}
	else if (v3.fragPos.z < 0.0)
	{
		clipWithOneVertexOutside(v3, v1, v2, trisNum);
		return true;
	}
	else
	{
		return false;
	}
}

void DeviceContext::clipWithOneVertexOutside(VertexOut& v1, VertexOut& v2, VertexOut& v3, int& trisNum)
{
	double alphaA = (-v1.fragPos.z) / (v2.fragPos.z - v1.fragPos.z);
	double alphaB = (-v1.fragPos.z) / (v3.fragPos.z - v1.fragPos.z);
	
	VertexOut v1a = Lerp(v1, v2, alphaA);
	VertexOut v1b = Lerp(v1, v3, alphaB);

	Vec4 vec1 = v2.fragPos - v1.fragPos;
	Vec4 vec2 = v3.fragPos - v1.fragPos;

	Vec4 nVec1 = v2.fragPos - v1a.fragPos;
	Vec4 nVec2 = v3.fragPos - v1a.fragPos;
	if ((vec1.cross(vec2).normalize()).dot(nVec1.cross(nVec2).normalize()) < 0.0)
	{
		postProcessTriangle(v1a, v2, v3, trisNum);
		postProcessTriangle(v1b, v1a, v3, trisNum);
	}
	else
	{
		postProcessTriangle(v1a, v3, v2, trisNum);
		postProcessTriangle(v1b, v3, v1a, trisNum);
	}
}

void DeviceContext::clipWithTwoVertexOutside(VertexOut& v1, VertexOut& v2, VertexOut& v3, int& trisNum)
{
	double alphaA = (-v1.fragPos.z) / (v3.fragPos.z - v1.fragPos.z);
	double alphaB = (-v2.fragPos.z) / (v3.fragPos.z - v2.fragPos.z);

	VertexOut v1n = Lerp(v1, v3, alphaA);
	VertexOut v2n = Lerp(v2, v3, alphaB);
	
	Vec4 vec1 = v2n.fragPos - v1n.fragPos;
	Vec4 vec2 = v3.fragPos - v1n.fragPos;

	Vec4 nVec1 = v2n.fragPos - v1n.fragPos;
	Vec4 nVec2 = v3.fragPos - v1n.fragPos;
	if ((vec1.cross(vec2).normalize()).dot(nVec1.cross(nVec2).normalize()) > 0.0)
	{
		postProcessTriangle(v1n, v2n, v3, trisNum);
	}
	else
	{
		postProcessTriangle(v1n, v3, v2n, trisNum);
	}
}

void DeviceContext::postProcessTriangle(VertexOut v1, VertexOut v2, VertexOut v3, int& trisNum)
{
	perspectiveCorrection(v1);
	perspectiveCorrection(v2);
	perspectiveCorrection(v3);

	toNDC(v1.fragPos);
	toNDC(v2.fragPos);
	toNDC(v3.fragPos);
	toNDC(v1.fragPosLightSpace);
	toNDC(v2.fragPosLightSpace);
	toNDC(v3.fragPosLightSpace);

	//if (backFaceCulling(v1, v2, v3, m_shader->m_obj->isClockwise) == false)
		//return;

	trisNum += 1;

	transformToScreen(v1.fragPos);
	transformToScreen(v2.fragPos);
	transformToScreen(v3.fragPos);
	transformToScreen(v1.fragPosLightSpace);
	transformToScreen(v2.fragPosLightSpace);
	transformToScreen(v3.fragPosLightSpace);

	drawTriangle(v1, v2, v3);
}

VertexOut DeviceContext::shadowTransformToProj(const VertexIn& v)
{
	VertexOut out = m_shader->shadowVertexShader(v);

	out.oneOverZ = 1.0 / out.fragPos.z;

	return out;
}

//转换到屏幕坐标
void DeviceContext::transformToScreen(Vec4& v)
{
	v.x = (v.x + 1.0)*m_device->getClientWidth()*0.5;
	v.y = (1.0 - v.y)*m_device->getClientHeight()*0.5;
}

//背面消隐
bool DeviceContext::backFaceCulling(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3, bool isClockwise)
{
	//if (m_renderMode == RENDER_MODE::WIREFRAME)
	//{
		//return true;
	//}
	//else
	{
		Vec4 vector1 = (v2.worldPos - v1.worldPos).normalize();
		Vec4 vector2 = (v3.worldPos - v1.worldPos).normalize();
		//顶点缓存中顺序为顺时针
		//叉积得到的方向与右手系一致
		Vec4 normal = vector1.cross(vector2);

		Vec4 viewDir = (m_shader->m_camera.eyePos - v1.worldPos).normalize();
		if (isClockwise)
		{
			if (normal.dot(viewDir) >= 0.0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (normal.dot(viewDir) <= 0.0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

void DeviceContext::drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	//线框模式
	if (m_renderMode == RENDER_MODE::WIREFRAME)
	{
		drawLine((int)v1.fragPos.x, (int)v1.fragPos.y, (int)v2.fragPos.x, (int)v2.fragPos.y);
		drawLine((int)v1.fragPos.x, (int)v1.fragPos.y, (int)v3.fragPos.x, (int)v3.fragPos.y);
		drawLine((int)v2.fragPos.x, (int)v2.fragPos.y, (int)v3.fragPos.x, (int)v3.fragPos.y);
	}
	else
	{
		m_shader->render_mode = m_renderMode;
		triangleRasterization(v1, v2, v3);
	}

}

// DDA
void DeviceContext::drawLine(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int epsl = std::abs(dx) >= std::abs(dy) ? std::abs(dx) : std::abs(dy);

	double stepX = (double)dx / epsl;
	double stepY = (double)dy / epsl;

	double x = x1;
	double y = y1;
	for (int i = 0; i <= epsl; i++)
	{
		m_device->DrawPixel((int)x, (int)y, Vec4(0.0, 1.0, 0.0, 1.0));
		x += stepX;
		y += stepY;
	}

}


void DeviceContext::triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	std::vector<VertexOut> vertices{ v1,v2,v3 };
	std::sort(vertices.begin(), vertices.end(), [](VertexOut v1, VertexOut v2) { return v1.fragPos.y < v2.fragPos.y; });
	VertexOut top = vertices[0];
	VertexOut middle = vertices[1];
	VertexOut bottom = vertices[2];

	drawScanLine(bottom, middle, top);
	
}

//shadow map绘制
void DeviceContext::shadowTriangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	std::vector<VertexOut> vertices{ v1,v2,v3 };
	std::sort(vertices.begin(), vertices.end(), [](VertexOut v1, VertexOut v2) { return v1.fragPos.y < v2.fragPos.y; });
	VertexOut top = vertices[0];
	VertexOut middle = vertices[1];
	VertexOut bottom = vertices[2];

	shadowdrawScanLine(bottom, middle, top);

}


Vec4 DeviceContext::barycentric(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec4 &P)
{
	Vec4 v1 = { c.x - a.x,b.x - a.x,a.x - P.x };
	Vec4 v2 = { c.y - a.y,b.y - a.y,a.y - P.y };

	Vec4 u = v1.cross(v2);
	if (std::abs(u.z) > 1e-2)
		return Vec4(1.0 - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec4(-1, 1, 1, 0);
}

//重心坐标算法
void DeviceContext::drawScanLine(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double xl = v1.fragPos.x;
	if (v2.fragPos.x < xl)xl = v2.fragPos.x;
	if (v3.fragPos.x < xl)xl = v3.fragPos.x;

	double xr = v1.fragPos.x;
	if (v2.fragPos.x > xr)xr = v2.fragPos.x;
	if (v3.fragPos.x > xr)xr = v3.fragPos.x;

	double yt = v1.fragPos.y;
	if (v2.fragPos.y > yt)yt = v2.fragPos.y;
	if (v3.fragPos.y < yt)yt = v3.fragPos.y;

	double yb = v1.fragPos.y;
	if (v2.fragPos.y > yb)yb = v2.fragPos.y;
	if (v3.fragPos.y > yb)yb = v3.fragPos.y;

	int xMin = (int)(xl + 0.5);
	int xMax = (int)(xr + 0.5);
	int yMin = (int)(yt + 0.5);
	int yMax = (int)(yb + 0.5);

	for (int x = xMin; x <= xMax; x++)
	{
		for (int y = yMin; y <= yMax; y++)
		{
			//裁剪
			if (x<0 || x>m_device->getClientWidth() || y<0 || y>m_device->getClientHeight())
				continue;
			Vec4 ret = barycentric(v1.fragPos, v2.fragPos, v3.fragPos, Vec4((double)x, (double)y, 0.0, 0.0));
			if (ret.x < 0 || ret.y < 0 || ret.z < 0)continue;
			
			VertexOut out;
			out.worldPos.x = v1.worldPos.x*ret.x + v2.worldPos.x*ret.y + v3.worldPos.x*ret.z;
			out.worldPos.y = v1.worldPos.y*ret.x + v2.worldPos.y*ret.y + v3.worldPos.y*ret.z;
			out.worldPos.z = v1.worldPos.z*ret.x + v2.worldPos.z*ret.y + v3.worldPos.z*ret.z;
			out.fragPos.x = v1.fragPos.x*ret.x + v2.fragPos.x*ret.y + v3.fragPos.x*ret.z;
			out.fragPos.y = v1.fragPos.y*ret.x + v2.fragPos.y*ret.y + v3.fragPos.y*ret.z;
			out.fragPos.z = v1.fragPos.z*ret.x + v2.fragPos.z*ret.y + v3.fragPos.z*ret.z;
			out.normal.x = v1.normal.x*ret.x + v2.normal.x*ret.y + v3.normal.x*ret.z;
			out.normal.y = v1.normal.y*ret.x + v2.normal.y*ret.y + v3.normal.y*ret.z;
			out.normal.z = v1.normal.z*ret.x + v2.normal.z*ret.y + v3.normal.z*ret.z;
			out.color.x = v1.color.x*ret.x + v2.color.x*ret.y + v3.color.x*ret.z;
			out.color.y = v1.color.y*ret.x + v2.color.y*ret.y + v3.color.y*ret.z;
			out.color.z = v1.color.z*ret.x + v2.color.z*ret.y + v3.color.z*ret.z;
			out.tex.x = v1.tex.x*ret.x + v2.tex.x*ret.y + v3.tex.x*ret.z;
			out.tex.y = v1.tex.y*ret.x + v2.tex.y*ret.y + v3.tex.y*ret.z;
			out.oneOverZ = v1.oneOverZ*ret.x + v2.oneOverZ*ret.y + v3.oneOverZ*ret.z;
			//=====
			out.vtangent.x = v1.vtangent.x * ret.x + v2.vtangent.x * ret.y + v3.vtangent.x * ret.z;
			out.vtangent.y = v1.vtangent.y * ret.x + v2.vtangent.y * ret.y + v3.vtangent.y * ret.z;
			out.vtangent.z = v1.vtangent.z * ret.x + v2.vtangent.z * ret.y + v3.vtangent.z * ret.z;
			out.vbitanget.x = v1.vbitanget.x * ret.x + v2.vbitanget.x * ret.y + v3.vbitanget.x * ret.z;
			out.vbitanget.y = v1.vbitanget.y * ret.x + v2.vbitanget.y * ret.y + v3.vbitanget.y * ret.z;
			out.vbitanget.z = v1.vbitanget.z * ret.x + v2.vbitanget.z * ret.y + v3.vbitanget.z * ret.z;
			out.vnormal.x = v1.vnormal.x * ret.x + v2.vnormal.x * ret.y + v3.vnormal.x * ret.z;
			out.vnormal.y = v1.vnormal.y * ret.x + v2.vnormal.y * ret.y + v3.vnormal.y * ret.z;
			out.vnormal.z = v1.vnormal.z * ret.x + v2.vnormal.z * ret.y + v3.vnormal.z * ret.z;
			//out.TBN = Mat3(out.vtangent, out.vbitanget, out.vnormal);
			//=====
			out.fragPosLightSpace.x = v1.fragPosLightSpace.x * ret.x + v2.fragPosLightSpace.x * ret.y + v3.fragPosLightSpace.x * ret.z;
			out.fragPosLightSpace.y = v1.fragPosLightSpace.y * ret.x + v2.fragPosLightSpace.y * ret.y + v3.fragPosLightSpace.y * ret.z;
			out.fragPosLightSpace.z = v1.fragPosLightSpace.z * ret.x + v2.fragPosLightSpace.z * ret.y + v3.fragPosLightSpace.z * ret.z;
			out.soneOverZ = v1.soneOverZ*ret.x + v2.soneOverZ*ret.y + v3.soneOverZ*ret.z;

			double curooz = m_device->getZ(x, y);
			if (out.oneOverZ >= curooz)
			{
				m_device->setZ(x, y, out.oneOverZ);
				//透视矫正
				double w = 1.0 / out.oneOverZ;
				out.tex.x *= w;
				out.tex.y *= w;
				out.normal.x *= w;
				out.normal.y *= w;
				out.normal.z *= w;
				out.color.x *= w;
				out.color.y *= w;
				out.color.z *= w;

				if (m_curObjectState == true)
				{
					m_device->DrawPixel(x, y, Vec4(1.0, 1.0, 1.0));
					continue;
				}

				//画像素点颜色
				m_device->DrawPixel(x, y, m_shader->pixelShader(out, m_device->getDepthMap()));
			}
		}
	}
}

void DeviceContext::shadowdrawScanLine(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double xl = v1.fragPos.x;
	if (v2.fragPos.x < xl)xl = v2.fragPos.x;
	if (v3.fragPos.x < xl)xl = v3.fragPos.x;

	double xr = v1.fragPos.x;
	if (v2.fragPos.x > xr)xr = v2.fragPos.x;
	if (v3.fragPos.x > xr)xr = v3.fragPos.x;

	double yt = v1.fragPos.y;
	if (v2.fragPos.y > yt)yt = v2.fragPos.y;
	if (v3.fragPos.y < yt)yt = v3.fragPos.y;

	double yb = v1.fragPos.y;
	if (v2.fragPos.y > yb)yb = v2.fragPos.y;
	if (v3.fragPos.y > yb)yb = v3.fragPos.y;

	int xMin = (int)(xl + 0.5);
	int xMax = (int)(xr + 0.5);
	int yMin = (int)(yt + 0.5);
	int yMax = (int)(yb + 0.5);

	for (int x = xMin; x <= xMax; x++)
	{
		for (int y = yMin; y <= yMax; y++)
		{
			//裁剪
			if (x<0 || x>m_device->getClientWidth() || y<0 || y>m_device->getClientHeight())
				continue;
			Vec4 ret = barycentric(v1.fragPos, v2.fragPos, v3.fragPos, Vec4((double)x, (double)y, 0.0, 0.0));
			if (ret.x < 0 || ret.y < 0 || ret.z < 0)continue;

			VertexOut out;
			out.oneOverZ = v1.oneOverZ*ret.x + v2.oneOverZ*ret.y + v3.oneOverZ*ret.z;

			double curooz = m_device->getDepth(x, y);
			if (out.oneOverZ >= curooz)
			{
				m_device->setDepth(x, y, out.oneOverZ);		
				/*
				double c = m_device->getDepth(x, y)*5;
				Vec4 color(c, c, c);
				m_device->DrawPixel(x, y, color);
				*/
			}
		}
	}
}