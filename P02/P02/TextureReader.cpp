#include "TextureReader.h"

#include <fstream>
#include <windows.h>
#include <fstream>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

TextureReader::TextureReader()
{
	texture_buffer = nullptr;
	normal_buffer = nullptr;
}

void TextureReader::textureToColorArray(std::wstring infilename)
{
	GdiplusStartupInput gdiplusstartinput;
	ULONG_PTR gdiplustoken;

	GdiplusStartup(&gdiplustoken, &gdiplusstartinput, NULL);

	//std::string outfilename("../outTex.txt");

	Bitmap* bmp = new Bitmap(infilename.c_str());
	UINT height = bmp->GetHeight();
	UINT width = bmp->GetWidth();
	
	Color color;
	//std::ofstream fout(outfilename.c_str());
	texture_buffer = new Texture(width, height);
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			bmp->GetPixel(x, y, &color);
			double r = static_cast<double>(color.GetRed())/255.0;
			double g = static_cast<double>(color.GetGreen())/255.0;
			double b = static_cast<double>(color.GetBlue())/255.0;
			texture_buffer->m_texMap[x][height - 1 - y] = Vec4(r, g, b, 1.0);
		}
	}
	delete bmp;
	GdiplusShutdown(gdiplustoken);

}

void TextureReader::textureToNormalArray(std::wstring infilename)
{
	GdiplusStartupInput gdiplusstartinput;
	ULONG_PTR gdiplustoken;

	GdiplusStartup(&gdiplustoken, &gdiplusstartinput, NULL);

	Bitmap* bmp = new Bitmap(infilename.c_str());
	UINT height = bmp->GetHeight();
	UINT width = bmp->GetWidth();

	Color color;

	normal_buffer = new Texture(width, height);
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			bmp->GetPixel(x, y, &color);
			double nx = (static_cast<double>(color.GetRed()) / 255.0 * 2.0 - 1.0);
			double ny = (static_cast<double>(color.GetGreen()) / 255.0 * 2.0 - 1.0);
			double nz = (static_cast<double>(color.GetBlue()) / 255.0 * 2.0 - 1.0);
			normal_buffer->m_texMap[x][height - 1 - y] = Vec4(nx, ny, nz, 0.0);
		}
	}
	delete bmp;
	GdiplusShutdown(gdiplustoken);
}