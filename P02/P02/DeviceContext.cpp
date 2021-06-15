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

//������Ⱦģʽ
void DeviceContext::setRenderMode(RENDER_MODE mode)
{
	m_renderMode = mode;
}

//������ɫ��
void DeviceContext::setShader(Shader* s)
{
	m_shader = s;
}

//���ƶ��㻺���е�������
void DeviceContext::drawIndexed()
{
	//�õ���Ļ�任����
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
	v.fragPos.x /= v.fragPos.w;
	v.fragPos.y /= v.fragPos.w;
	v.fragPos.z /= v.fragPos.w;
	v.fragPos.w = 1;
}

//��cvv�ü�
bool DeviceContext::clip(VertexOut& v)
{
	//cvvΪ x-1,1  y-1,1  z0,1
	if (v.fragPos.x >= -v.fragPos.w && v.fragPos.x <= v.fragPos.w &&
		v.fragPos.y >= -v.fragPos.w && v.fragPos.y <= v.fragPos.w &&
		v.fragPos.z >= 0.0 && v.fragPos.z <= v.fragPos.w)
	{
		return true;
	}
	return false;	
}

//ת����βü��ռ�
VertexOut DeviceContext::transformToProj(const VertexIn& v)
{
	VertexOut out = m_shader->vertexShader(v);
	//����oneOverZ
	out.oneOverZ = 1 / out.fragPos.w;
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
	v.fragPos = v.fragPos * m;
}

//��������
bool DeviceContext::backFaceCulling(const VertexIn& v1, const VertexIn& v2, const VertexIn& v3)
{
	//�߿�ģʽ�����б�������
	if (m_renderMode == RENDER_MODE::WIREFRAME)
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

		Vec4 viewDir = v1.pos - m_shader->m_camera.eyePos;
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
//ɨ�������
//left ��˵�  right �Ҷ˵�
void DeviceContext::scanLineFill(const VertexOut& left, const VertexOut& right, int yIndex)
{
	double dx = right.fragPos.x - left.fragPos.x;

	for (double x = left.fragPos.x; x <= right.fragPos.x; x += 1.0)
	{
		//��������
		int xIndex = static_cast<int>(x + 0.5);

		if (xIndex >= 0 && xIndex < m_device->getClientWidth())
		{
			//��ֵϵ��
			double lerpFactor = 0;
			if (dx != 0)
			{
				lerpFactor = (x - left.fragPos.x) / dx;
			}

			//��Ȳ���
			//1/z����x����y'�����Թ�ϵ��
			double oneOverZ = Lerp(left.oneOverZ, right.oneOverZ, lerpFactor);
			if (oneOverZ >= m_device->getZ(xIndex, yIndex))
			{
				m_device->setZ(xIndex, yIndex, oneOverZ);

				double w = 1 / oneOverZ;
				//��ֵ���� ԭ����Ҫ��ֵ����Ϣ������oneOverZ
				//���ڵõ���ֵ�����Ϣ��Ҫ����oneOverZ�õ���ʵֵ
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
				//�����ص���ɫ
				m_device->DrawPixel(xIndex, yIndex, m_shader->pixelShader(out));
			}
		}
	}
}

//��ƽ�������� v3Ϊ�¶���
//y����ÿ������һ������ ����y��ֵ����

void DeviceContext::drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//ÿ��y����һ������
	for (double y = v1.fragPos.y; y <= v3.fragPos.y; y += 1.0)
	{
		//��������
		int yIndex = static_cast<int>(y + 0.5f);
		if (yIndex >= 0 && yIndex < m_device->getClientHeight())
		{
			double t = dy / (v3.fragPos.y - v1.fragPos.y);

			//��ֵ�������Ҷ���
			VertexOut new1 = Lerp(v1, v3, t);
			VertexOut new2 = Lerp(v2, v3, t);
			dy += 1.0;
			//ɨ�������
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

//��ƽ�������� v1Ϊ�϶���
void DeviceContext::drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	double dy = 0;//ÿ��y����һ������
	for (double y = v1.fragPos.y; y <= v2.fragPos.y; y += 1.0)
	{
		//��������
		int yIndex = static_cast<int>(y + 0.5f);

		if (yIndex >= 0 && yIndex < m_device->getClientHeight())
		{
			double t = dy / (v2.fragPos.y - v1.fragPos.y);

			//��ֵ���Ҷ���
			VertexOut new1 = Lerp(v1, v2, t);
			VertexOut new2 = Lerp(v1, v3, t);
			dy += 1.0;

			//ɨ�������
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

//��դ��������
void DeviceContext::triangleRasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	//�ж��Ƿ���ƽ�׻���ƽ��������
	if (v1.fragPos.y == v2.fragPos.y)
	{
		if (v1.fragPos.y < v3.fragPos.y)
		{//ƽ��
			drawTriangleTop(v1, v2, v3);
		}
		else
		{//ƽ��
			drawTriangleBottom(v3, v1, v2);
		}
	}
	else if (v1.fragPos.y == v3.fragPos.y)
	{
		if (v1.fragPos.y < v2.fragPos.y)
		{//ƽ��
			drawTriangleTop(v1, v3, v2);
		}
		else
		{//ƽ��
			drawTriangleBottom(v2, v1, v3);
		}
	}
	else if (v2.fragPos.y == v3.fragPos.y)
	{
		if (v2.fragPos.y < v1.fragPos.y)
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
			return v1.fragPos.y < v2.fragPos.y; });
		VertexOut top = vertices[0];
		VertexOut middle = vertices[1];
		VertexOut bottom = vertices[2];

		//��ֵ���м��
		double middleX = (middle.fragPos.y - top.fragPos.y) * (bottom.fragPos.x - top.fragPos.x) /
			(bottom.fragPos.y - top.fragPos.y) + top.fragPos.x;
		double dy = middle.fragPos.y - top.fragPos.y;
		double t = dy / (bottom.fragPos.y - top.fragPos.y);

		VertexOut newMiddle = Lerp(top, bottom, t);
		newMiddle.fragPos.x = middleX;
		newMiddle.fragPos.y = middle.fragPos.y;

		//ƽ��
		drawTriangleTop(middle, newMiddle, bottom);
		//ƽ��
		drawTriangleBottom(top, middle, newMiddle);
	}
}

