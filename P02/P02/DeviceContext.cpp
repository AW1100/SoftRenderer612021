#include "DeviceContext.h"
#include <algorithm>
#include "Vertex.h"

using namespace Util;

DeviceContext::DeviceContext() :m_renderMode(RENDER_MODE::WIREFRAME)
{
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

//绘制顶点缓冲中的三角形
void DeviceContext::drawIndexed()
{
	//得到屏幕变换矩阵
	Mat4 screenTransformMat = screenTransform(m_device->getClientWidth(),
		m_device->getClientHeight());
	

	for (unsigned int i = 0; i < m_obj->indices.size(); i=i+9)
	{
		Vec2 uv1 = m_obj->uv[m_obj->indices[i + 1]-1]*2048;
		Vec2 uv2 = m_obj->uv[m_obj->indices[i + 4]-1]*2048;
		Vec2 uv3 = m_obj->uv[m_obj->indices[i + 7]-1]*2048;
		int uv1Loc = static_cast<int>(uv1.y * 2048) + static_cast<int>(uv1.x);
		int uv2Loc = static_cast<int>(uv2.y * 2048) + static_cast<int>(uv2.x);
		int uv3Loc = static_cast<int>(uv3.y * 2048) + static_cast<int>(uv3.x);
		//Vec4 c(0.5, 0.5, 0.5);
		VertexIn p1(m_obj->pos[m_obj->indices[i] - 1], m_texture.colorArray[uv1Loc], m_obj->normal[m_obj->indices[i + 2] - 1]);
		VertexIn p2(m_obj->pos[m_obj->indices[i+3] - 1], m_texture.colorArray[uv2Loc], m_obj->normal[m_obj->indices[i + 5] - 1]);
		VertexIn p3(m_obj->pos[m_obj->indices[i+6] - 1], m_texture.colorArray[uv3Loc], m_obj->normal[m_obj->indices[i + 8] - 1]);
		//背面消隐
		if (backFaceCulling(p1, p2, p3) == false)
		{
			//continue;
		}

		//转换到齐次裁剪空间，即投影后的坐标
		VertexOut v1 = transformToProj(p1);
		VertexOut v2 = transformToProj(p2);
		VertexOut v3 = transformToProj(p3);


		//判断是否通过cvv裁剪
		
		if (clip(v1) == false || clip(v2) == false || clip(v3) == false)
		{
			continue;
		}
		
		//进行透视除法 转到cvv
		toCVV(v1);
		toCVV(v2);
		toCVV(v3);

		//将投影得到的坐标转化为屏幕坐标
		transformToScreen(screenTransformMat, v1);
		transformToScreen(screenTransformMat, v2);
		transformToScreen(screenTransformMat, v3);

		drawTriangle(v1, v2, v3);
	}
}

//转化到cvv
void DeviceContext::toCVV(VertexOut& v)
{
	v.fragPos.x /= v.fragPos.w;
	v.fragPos.y /= v.fragPos.w;
	v.fragPos.z /= v.fragPos.w;
	v.fragPos.w = 1;
}

//简单cvv裁剪
bool DeviceContext::clip(VertexOut& v)
{
	//cvv为 x-1,1  y-1,1  z0,1
	if (v.fragPos.x >= -v.fragPos.w && v.fragPos.x <= v.fragPos.w &&
		v.fragPos.y >= -v.fragPos.w && v.fragPos.y <= v.fragPos.w &&
		v.fragPos.z >= 0.0 && v.fragPos.z <= v.fragPos.w)
	{
		return true;
	}
	return false;	
}

//转到齐次裁剪空间
VertexOut DeviceContext::transformToProj(const VertexIn& v)
{
	VertexOut out = m_shader->vertexShader(v);
	//设置oneOverZ
	out.oneOverZ = 1 / out.fragPos.w;
	//由于1/z和x,y成线性关系
	//这里将需要插值的信息都乘以1/z 得到 s/z和t/z等，方便光栅化阶段进行插值
	out.color.x *= out.oneOverZ;
	out.color.y *= out.oneOverZ;
	out.color.z *= out.oneOverZ;

	out.normal.x *= out.oneOverZ;
	out.normal.y *= out.oneOverZ;
	out.normal.z *= out.oneOverZ;

	//out.tex.u *= out.oneOverZ;
	//out.tex.v *= out.oneOverZ;

	return out;
}

//转换到屏幕坐标
void DeviceContext::transformToScreen(const Mat4& m, VertexOut& v)
{
	v.fragPos = v.fragPos * m;
}

//背面消隐
bool DeviceContext::backFaceCulling(const VertexIn& v1, const VertexIn& v2, const VertexIn& v3)
{
	//线框模式不进行背面消隐
	if (m_renderMode == RENDER_MODE::WIREFRAME)
	{
		return true;
	}
	else
	{
		Vec4 vector1 = v2.pos - v1.pos;
		Vec4 vector2 = v3.pos - v2.pos;
		//顶点缓存中顺序为顺时针
		//叉积得到的方向与右手系一致
		Vec4 normal = vector1.cross(vector2);

		Vec4 viewDir = v1.pos - m_shader->m_camera.eyePos;
		if (normal.dot(viewDir) < 0)
		{
			return true;
		}
		return false;
	}
}

//画三角形
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

//bresenham
/*
void DeviceContext::drawLine(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int stepx = 1;
	int stepy = 1;

	if (dx >= 0)
	{
		stepx = 1;
	}
	else
	{
		stepx = -1;
		dx = abs(dx);
	}

	if (dy >= 0)
	{
		stepy = 1;
	}
	else
	{
		stepy = -1;
		dy = abs(dy);
	}

	int deltaX = 2 * dx;
	int deltaY = 2 * dy;
	if (dx > dy)
	{
		int error = deltaY - dx;
		for (int i = 0; i <= dx; ++i)
		{
			if (x1 >= 0 && x1 < m_device->getClientWidth() && y1 >= 0 && y1 < m_device->getClientHeight())
				m_device->DrawPixel(x1, y1, Vec4(1.0, 1.0, 1.0, 1.0));
			if (error >= 0)
			{
				error -= deltaX;
				y1 += stepy;
			}
			error += deltaY;
			x1 += stepx;
		}
	}
	else
	{
		int error = deltaX - dy;
		for (int i = 0; i <= dy; i++)
		{
			if (x1 >= 0 && x1 < m_device->getClientWidth() && y1 >= 0 && y1 < m_device->getClientHeight())
				m_device->DrawPixel(x1, y1, Vec4(1.0, 1.0, 1.0, 1.0));
			if (error >= 0)
			{
				error -= deltaY;
				x1 += stepx;
			}
			error += deltaX;
			y1 += stepy;
		}
	}
}
*/
//扫描线填充
//left 左端点  right 右端点
void DeviceContext::scanLineFill(const VertexOut& left, const VertexOut& right, int yIndex)
{
	double dx = right.fragPos.x - left.fragPos.x;

	for (double x = left.fragPos.x; x <= right.fragPos.x; x += 1.0)
	{
		//四舍五入
		int xIndex = static_cast<int>(x + 0.5);

		if (xIndex >= 0 && xIndex < m_device->getClientWidth())
		{
			//插值系数
			double lerpFactor = 0;
			if (dx != 0)
			{
				lerpFactor = (x - left.fragPos.x) / dx;
			}

			//深度测试
			//1/z’与x’和y'是线性关系的
			double oneOverZ = Lerp(left.oneOverZ, right.oneOverZ, lerpFactor);
			if (oneOverZ >= m_device->getZ(xIndex, yIndex))
			{
				m_device->setZ(xIndex, yIndex, oneOverZ);

				double w = 1 / oneOverZ;
				//插值顶点 原先需要插值的信息均乘以oneOverZ
				//现在得到插值后的信息需要除以oneOverZ得到真实值
				VertexOut out = Lerp(left, right, lerpFactor);
				out.fragPos.y = yIndex;
				//out.tex.u *= w;
				//out.tex.v *= w;
				out.normal.x *= w;
				out.normal.y *= w;
				out.normal.z *= w;
				out.color.x *= w;
				out.color.y *= w;
				out.color.z *= w;
				//画像素点颜色
				m_device->DrawPixel(xIndex, yIndex, m_shader->pixelShader(out));
			}
		}
	}
}

//画平顶三角形 v3为下顶点
//y方向每次增加一个像素 根据y插值顶点

void DeviceContext::drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//每次y增加一个像素
	for (double y = v1.fragPos.y; y <= v3.fragPos.y; y += 1.0)
	{
		//四舍五入
		int yIndex = static_cast<int>(y + 0.5f);
		if (yIndex >= 0 && yIndex < m_device->getClientHeight())
		{
			double t = dy / (v3.fragPos.y - v1.fragPos.y);

			//插值生成左右顶点
			VertexOut new1 = Lerp(v1, v3, t);
			VertexOut new2 = Lerp(v2, v3, t);
			dy += 1.0;
			//扫描线填充
			if (new1.fragPos.x < new2.fragPos.x)
			{
				scanLineFill(new1, new2, yIndex);
			}
			else
			{
				scanLineFill(new2, new1, yIndex);
			}
		}

	}
}

//画平底三角形 v1为上顶点
void DeviceContext::drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//每次y增加一个像素
	for (double y = v1.fragPos.y; y <= v2.fragPos.y; y += 1.0)
	{
		//四舍五入
		int yIndex = static_cast<int>(y + 0.5f);

		if (yIndex >= 0 && yIndex < m_device->getClientHeight())
		{
			double t = dy / (v2.fragPos.y - v1.fragPos.y);

			//插值左右顶点
			VertexOut new1 = Lerp(v1, v2, t);
			VertexOut new2 = Lerp(v1, v3, t);
			dy += 1.0;

			//扫描线填充
			if (new1.fragPos.x < new2.fragPos.x)
			{
				scanLineFill(new1, new2, yIndex);
			}
			else
			{
				scanLineFill(new2, new1, yIndex);
			}
		}
	}
}

//光栅化三角形
void DeviceContext::triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	//判断是否是平底或者平顶三角形
	if (v1.fragPos.y == v2.fragPos.y)
	{
		if (v1.fragPos.y < v3.fragPos.y)
		{//平顶
			drawTriangleTop(v1, v2, v3);
		}
		else
		{//平底
			drawTriangleBottom(v3, v1, v2);
		}
	}
	else if (v1.fragPos.y == v3.fragPos.y)
	{
		if (v1.fragPos.y < v2.fragPos.y)
		{//平顶
			drawTriangleTop(v1, v3, v2);
		}
		else
		{//平底
			drawTriangleBottom(v2, v1, v3);
		}
	}
	else if (v2.fragPos.y == v3.fragPos.y)
	{
		if (v2.fragPos.y < v1.fragPos.y)
		{//平顶
			drawTriangleTop(v2, v3, v1);
		}
		else
		{//平底
			drawTriangleBottom(v1, v2, v3);
		}
	}
	//一般三角形 将其分割成平底三角形和平顶三角形
	else
	{
		//根据y值将三个顶点排序
		std::vector<VertexOut> vertices{ v1,v2,v3 };
		std::sort(vertices.begin(), vertices.end(), [](VertexOut v1, VertexOut v2) {
			return v1.fragPos.y < v2.fragPos.y; });
		VertexOut top = vertices[0];
		VertexOut middle = vertices[1];
		VertexOut bottom = vertices[2];

		//插值求中间点
		double middleX = (middle.fragPos.y - top.fragPos.y) * (bottom.fragPos.x - top.fragPos.x) /
			(bottom.fragPos.y - top.fragPos.y) + top.fragPos.x;
		double dy = middle.fragPos.y - top.fragPos.y;
		double t = dy / (bottom.fragPos.y - top.fragPos.y);

		VertexOut newMiddle = Lerp(top, bottom, t);
		newMiddle.fragPos.x = middleX;
		newMiddle.fragPos.y = middle.fragPos.y;

		//平顶
		drawTriangleTop(middle, newMiddle, bottom);
		//平底
		drawTriangleBottom(top, middle, newMiddle);
	}
}

