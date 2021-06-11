#include "DeviceContext.h"
#include <algorithm>
#include "Vertex.h"
#include "Util.h"

using namespace Util;

DeviceContext::DeviceContext() :m_renderMode(WIREFRAME)
{
}


DeviceContext::~DeviceContext()
{
}

void DeviceContext::init(Device* pDevice)
{
	m_pDevice = pDevice;
}

//设置渲染模式
void DeviceContext::setRenderMode(RENDER_MODE mode)
{
	m_renderMode = mode;
}

//设置着色器
void DeviceContext::setShader(Shader* s)
{
	m_pShader = s;
}

//绘制顶点缓冲中的三角形
void DeviceContext::drawIndexed()
{
	//得到屏幕变换矩阵
	Mat4 screenTransformMat = screenTransform(m_pDevice->getClientWidth(),
		m_pDevice->getClientHeight());

	for (unsigned int i = 0; i < obj->indices.size(); i=i+6)
	{
		VertexIn p1(obj->pos[obj->indices[i] - 1], Vec4(0.0, 1.0, 1.0), obj->normal[obj->indices[i + 1] - 1]);
		VertexIn p2(obj->pos[obj->indices[i+2] - 1], Vec4(0.0, 1.0, 1.0), obj->normal[obj->indices[i + 3] - 1]);
		VertexIn p3(obj->pos[obj->indices[i+4] - 1], Vec4(0.0, 1.0, 1.0), obj->normal[obj->indices[i + 5] - 1]);
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
	v.postPos.x /= v.postPos.w;
	v.postPos.y /= v.postPos.w;
	v.postPos.z /= v.postPos.w;
	v.postPos.w = 1;
}

//简单cvv裁剪
bool DeviceContext::clip(const VertexOut& v)
{
	//cvv为 x-1,1  y-1,1  z0,1
	if (v.postPos.x >= -v.postPos.w && v.postPos.x <= v.postPos.w &&
		v.postPos.y >= -v.postPos.w && v.postPos.y <= v.postPos.w &&
		v.postPos.z >= 0.0 && v.postPos.z <= v.postPos.w)
	{
		return true;
	}
	return false;
}

//转到齐次裁剪空间
VertexOut DeviceContext::transformToProj(const VertexIn& v)
{
	VertexOut out = m_pShader->vertexShader(v);
	//设置oneOverZ
	out.oneOverZ = 1 / out.postPos.w;
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
	v.postPos = v.postPos * m;
}

//背面消隐
bool DeviceContext::backFaceCulling(const VertexIn& v1, const VertexIn& v2, const VertexIn& v3)
{
	//线框模式不进行背面消隐
	if (m_renderMode == WIREFRAME)
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

		Vec4 viewDir = v1.pos - m_pShader->camera.eyePos;
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
	if (m_renderMode == WIREFRAME)
	{
		drawLine(v1.postPos.x, v1.postPos.y, v2.postPos.x, v2.postPos.y);
		drawLine(v1.postPos.x, v1.postPos.y, v3.postPos.x, v3.postPos.y);
		drawLine(v2.postPos.x, v2.postPos.y, v3.postPos.x, v3.postPos.y);
	}
	else if (m_renderMode == SOLID)
	{
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
		m_pDevice->DrawPixel(x, y, Vec4(0.0, 1.0, 0.0, 1.0));
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
			if (x1 >= 0 && x1 < m_pDevice->getClientWidth() && y1 >= 0 && y1 < m_pDevice->getClientHeight())
				m_pDevice->DrawPixel(x1, y1, Vec4(1.0, 1.0, 1.0, 1.0));
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
			if (x1 >= 0 && x1 < m_pDevice->getClientWidth() && y1 >= 0 && y1 < m_pDevice->getClientHeight())
				m_pDevice->DrawPixel(x1, y1, Vec4(1.0, 1.0, 1.0, 1.0));
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
	double dx = right.postPos.x - left.postPos.x;

	for (double x = left.postPos.x; x <= right.postPos.x; x += 1.0)
	{
		//四舍五入
		int xIndex = static_cast<int>(x + 0.5);

		if (xIndex >= 0 && xIndex < m_pDevice->getClientWidth())
		{
			//插值系数
			double lerpFactor = 0;
			if (dx != 0)
			{
				lerpFactor = (x - left.postPos.x) / dx;
			}

			//深度测试
			//1/z’与x’和y'是线性关系的
			double oneOverZ = Lerp(left.oneOverZ, right.oneOverZ, lerpFactor);
			if (oneOverZ >= m_pDevice->getZ(xIndex, yIndex))
			{
				m_pDevice->setZ(xIndex, yIndex, oneOverZ);

				double w = 1 / oneOverZ;
				//插值顶点 原先需要插值的信息均乘以oneOverZ
				//现在得到插值后的信息需要除以oneOverZ得到真实值
				VertexOut out = Lerp(left, right, lerpFactor);
				out.postPos.y = yIndex;
				//out.tex.u *= w;
				//out.tex.v *= w;
				out.normal.x *= w;
				out.normal.y *= w;
				out.normal.z *= w;
				out.color.x *= w;
				out.color.y *= w;
				out.color.z *= w;
				//画像素点颜色
				m_pDevice->DrawPixel(xIndex, yIndex, m_pShader->pixelShader(out));
			}
		}
	}
}

//画平顶三角形 v3为下顶点
//y方向每次增加一个像素 根据y插值顶点

void DeviceContext::drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//每次y增加一个像素
	for (double y = v1.postPos.y; y <= v3.postPos.y; y += 1.0)
	{
		//四舍五入
		int yIndex = static_cast<int>(y + 0.5f);
		if (yIndex >= 0 && yIndex < m_pDevice->getClientHeight())
		{
			double t = dy / (v3.postPos.y - v1.postPos.y);

			//插值生成左右顶点
			VertexOut new1 = Lerp(v1, v3, t);
			VertexOut new2 = Lerp(v2, v3, t);
			dy += 1.0;
			//扫描线填充
			if (new1.postPos.x < new2.postPos.x)
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
	for (double y = v1.postPos.y; y <= v2.postPos.y; y += 1.0)
	{
		//四舍五入
		int yIndex = static_cast<int>(y + 0.5f);

		if (yIndex >= 0 && yIndex < m_pDevice->getClientHeight())
		{
			double t = dy / (v2.postPos.y - v1.postPos.y);

			//插值左右顶点
			VertexOut new1 = Lerp(v1, v2, t);
			VertexOut new2 = Lerp(v1, v3, t);
			dy += 1.0;

			//扫描线填充
			if (new1.postPos.x < new2.postPos.x)
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
	if (v1.postPos.y == v2.postPos.y)
	{
		if (v1.postPos.y < v3.postPos.y)
		{//平顶
			drawTriangleTop(v1, v2, v3);
		}
		else
		{//平底
			drawTriangleBottom(v3, v1, v2);
		}
	}
	else if (v1.postPos.y == v3.postPos.y)
	{
		if (v1.postPos.y < v2.postPos.y)
		{//平顶
			drawTriangleTop(v1, v3, v2);
		}
		else
		{//平底
			drawTriangleBottom(v2, v1, v3);
		}
	}
	else if (v2.postPos.y == v3.postPos.y)
	{
		if (v2.postPos.y < v1.postPos.y)
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
			return v1.postPos.y < v2.postPos.y; });
		VertexOut top = vertices[0];
		VertexOut middle = vertices[1];
		VertexOut bottom = vertices[2];

		//插值求中间点
		double middleX = (middle.postPos.y - top.postPos.y) * (bottom.postPos.x - top.postPos.x) /
			(bottom.postPos.y - top.postPos.y) + top.postPos.x;
		double dy = middle.postPos.y - top.postPos.y;
		double t = dy / (bottom.postPos.y - top.postPos.y);

		VertexOut newMiddle = Lerp(top, bottom, t);
		newMiddle.postPos.x = middleX;
		newMiddle.postPos.y = middle.postPos.y;

		//平顶
		drawTriangleTop(middle, newMiddle, bottom);
		//平底
		drawTriangleBottom(top, middle, newMiddle);
	}
}

