#include "TextureReader.h"

//#include <gdiplus.h>
#include <fstream>
#include <windows.h>
#include <fstream>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

void TextureReader::textureToColorArray()
{
	GdiplusStartupInput gdiplusstartinput;
	ULONG_PTR gdiplustoken;

	GdiplusStartup(&gdiplustoken, &gdiplusstartinput, NULL);

	std::wstring infilename(L"../msTex.bmp");
	//std::string outfilename("../outTex.txt");

	Bitmap* bmp = new Bitmap(infilename.c_str());
	UINT height = bmp->GetHeight();
	UINT width = bmp->GetWidth();
	
	Color color;
	//std::ofstream fout(outfilename.c_str());

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			bmp->GetPixel(x, y, &color);
			double r = static_cast<double>(color.GetRed())/255.0;
			double g = static_cast<double>(color.GetGreen())/255.0;
			double b = static_cast<double>(color.GetBlue())/255.0;
			colorArray.push_back(Vec4(r, g, b));
		}
	}
	delete bmp;
	GdiplusShutdown(gdiplustoken);

}