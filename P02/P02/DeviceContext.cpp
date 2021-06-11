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

//������Ⱦģʽ
void DeviceContext::setRenderMode(RENDER_MODE mode)
{
	m_renderMode = mode;
}

//������ɫ��
void DeviceContext::setShader(Shader* s)
{
	m_pShader = s;
}

//���ƶ��㻺���е�������
void DeviceContext::drawIndexed()
{
	//�õ���Ļ�任����
	Mat4 screenTransformMat = screenTransform(m_pDevice->getClientWidth(),
		m_pDevice->getClientHeight());

	for (unsigned int i = 0; i < obj->indices.size(); i=i+6)
	{
		VertexIn p1(obj->pos[obj->indices[i] - 1], Vec4(0.0, 1.0, 1.0), obj->normal[obj->indices[i + 1] - 1]);
		VertexIn p2(obj->pos[obj->indices[i+2] - 1], Vec4(0.0, 1.0, 1.0), obj->normal[obj->indices[i + 3] - 1]);
		VertexIn p3(obj->pos[obj->indices[i+4] - 1], Vec4(0.0, 1.0, 1.0), obj->normal[obj->indices[i + 5] - 1]);
		//��������
		if (backFaceCulling(p1, p2, p3) == false)
		{
			//continue;
		}

		//ת������βü��ռ䣬��ͶӰ�������
		VertexOut v1 = transformToProj(p1);
		VertexOut v2 = transformToProj(p2);
		VertexOut v3 = transformToProj(p3);


		//�ж��Ƿ�ͨ��cvv�ü�
		
		if (clip(v1) == false || clip(v2) == false || clip(v3) == false)
		{
			continue;
		}
		
		//����͸�ӳ��� ת��cvv
		toCVV(v1);
		toCVV(v2);
		toCVV(v3);

		//��ͶӰ�õ�������ת��Ϊ��Ļ����
		transformToScreen(screenTransformMat, v1);
		transformToScreen(screenTransformMat, v2);
		transformToScreen(screenTransformMat, v3);

		drawTriangle(v1, v2, v3);
	}
}

//ת����cvv
void DeviceContext::toCVV(VertexOut& v)
{
	v.postPos.x /= v.postPos.w;
	v.postPos.y /= v.postPos.w;
	v.postPos.z /= v.postPos.w;
	v.postPos.w = 1;
}

//��cvv�ü�
bool DeviceContext::clip(const VertexOut& v)
{
	//cvvΪ x-1,1  y-1,1  z0,1
	if (v.postPos.x >= -v.postPos.w && v.postPos.x <= v.postPos.w &&
		v.postPos.y >= -v.postPos.w && v.postPos.y <= v.postPos.w &&
		v.postPos.z >= 0.0 && v.postPos.z <= v.postPos.w)
	{
		return true;
	}
	return false;
}

//ת����βü��ռ�
VertexOut DeviceContext::transformToProj(const VertexIn& v)
{
	VertexOut out = m_pShader->vertexShader(v);
	//����oneOverZ
	out.oneOverZ = 1 / out.postPos.w;
	//����1/z��x,y�����Թ�ϵ
	//���ｫ��Ҫ��ֵ����Ϣ������1/z �õ� s/z��t/z�ȣ������դ���׶ν��в�ֵ
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

//ת������Ļ����
void DeviceContext::transformToScreen(const Mat4& m, VertexOut& v)
{
	v.postPos = v.postPos * m;
}

//��������
bool DeviceContext::backFaceCulling(const VertexIn& v1, const VertexIn& v2, const VertexIn& v3)
{
	//�߿�ģʽ�����б�������
	if (m_renderMode == WIREFRAME)
	{
		return true;
	}
	else
	{
		Vec4 vector1 = v2.pos - v1.pos;
		Vec4 vector2 = v3.pos - v2.pos;
		//���㻺����˳��Ϊ˳ʱ��
		//����õ��ķ���������ϵһ��
		Vec4 normal = vector1.cross(vector2);

		Vec4 viewDir = v1.pos - m_pShader->camera.eyePos;
		if (normal.dot(viewDir) < 0)
		{
			return true;
		}
		return false;
	}
}

//��������
void DeviceContext::drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	//�߿�ģʽ
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
//ɨ�������
//left ��˵�  right �Ҷ˵�
void DeviceContext::scanLineFill(const VertexOut& left, const VertexOut& right, int yIndex)
{
	double dx = right.postPos.x - left.postPos.x;

	for (double x = left.postPos.x; x <= right.postPos.x; x += 1.0)
	{
		//��������
		int xIndex = static_cast<int>(x + 0.5);

		if (xIndex >= 0 && xIndex < m_pDevice->getClientWidth())
		{
			//��ֵϵ��
			double lerpFactor = 0;
			if (dx != 0)
			{
				lerpFactor = (x - left.postPos.x) / dx;
			}

			//��Ȳ���
			//1/z����x����y'�����Թ�ϵ��
			double oneOverZ = Lerp(left.oneOverZ, right.oneOverZ, lerpFactor);
			if (oneOverZ >= m_pDevice->getZ(xIndex, yIndex))
			{
				m_pDevice->setZ(xIndex, yIndex, oneOverZ);

				double w = 1 / oneOverZ;
				//��ֵ���� ԭ����Ҫ��ֵ����Ϣ������oneOverZ
				//���ڵõ���ֵ�����Ϣ��Ҫ����oneOverZ�õ���ʵֵ
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
				//�����ص���ɫ
				m_pDevice->DrawPixel(xIndex, yIndex, m_pShader->pixelShader(out));
			}
		}
	}
}

//��ƽ�������� v3Ϊ�¶���
//y����ÿ������һ������ ����y��ֵ����

void DeviceContext::drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//ÿ��y����һ������
	for (double y = v1.postPos.y; y <= v3.postPos.y; y += 1.0)
	{
		//��������
		int yIndex = static_cast<int>(y + 0.5f);
		if (yIndex >= 0 && yIndex < m_pDevice->getClientHeight())
		{
			double t = dy / (v3.postPos.y - v1.postPos.y);

			//��ֵ�������Ҷ���
			VertexOut new1 = Lerp(v1, v3, t);
			VertexOut new2 = Lerp(v2, v3, t);
			dy += 1.0;
			//ɨ�������
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

//��ƽ�������� v1Ϊ�϶���
void DeviceContext::drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//ÿ��y����һ������
	for (double y = v1.postPos.y; y <= v2.postPos.y; y += 1.0)
	{
		//��������
		int yIndex = static_cast<int>(y + 0.5f);

		if (yIndex >= 0 && yIndex < m_pDevice->getClientHeight())
		{
			double t = dy / (v2.postPos.y - v1.postPos.y);

			//��ֵ���Ҷ���
			VertexOut new1 = Lerp(v1, v2, t);
			VertexOut new2 = Lerp(v1, v3, t);
			dy += 1.0;

			//ɨ�������
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

//��դ��������
void DeviceContext::triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	//�ж��Ƿ���ƽ�׻���ƽ��������
	if (v1.postPos.y == v2.postPos.y)
	{
		if (v1.postPos.y < v3.postPos.y)
		{//ƽ��
			drawTriangleTop(v1, v2, v3);
		}
		else
		{//ƽ��
			drawTriangleBottom(v3, v1, v2);
		}
	}
	else if (v1.postPos.y == v3.postPos.y)
	{
		if (v1.postPos.y < v2.postPos.y)
		{//ƽ��
			drawTriangleTop(v1, v3, v2);
		}
		else
		{//ƽ��
			drawTriangleBottom(v2, v1, v3);
		}
	}
	else if (v2.postPos.y == v3.postPos.y)
	{
		if (v2.postPos.y < v1.postPos.y)
		{//ƽ��
			drawTriangleTop(v2, v3, v1);
		}
		else
		{//ƽ��
			drawTriangleBottom(v1, v2, v3);
		}
	}
	//һ�������� ����ָ��ƽ�������κ�ƽ��������
	else
	{
		//����yֵ��������������
		std::vector<VertexOut> vertices{ v1,v2,v3 };
		std::sort(vertices.begin(), vertices.end(), [](VertexOut v1, VertexOut v2) {
			return v1.postPos.y < v2.postPos.y; });
		VertexOut top = vertices[0];
		VertexOut middle = vertices[1];
		VertexOut bottom = vertices[2];

		//��ֵ���м��
		double middleX = (middle.postPos.y - top.postPos.y) * (bottom.postPos.x - top.postPos.x) /
			(bottom.postPos.y - top.postPos.y) + top.postPos.x;
		double dy = middle.postPos.y - top.postPos.y;
		double t = dy / (bottom.postPos.y - top.postPos.y);

		VertexOut newMiddle = Lerp(top, bottom, t);
		newMiddle.postPos.x = middleX;
		newMiddle.postPos.y = middle.postPos.y;

		//ƽ��
		drawTriangleTop(middle, newMiddle, bottom);
		//ƽ��
		drawTriangleBottom(top, middle, newMiddle);
	}
}

