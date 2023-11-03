#include "Light.h"
#include "Util.h"

#include <algorithm>

Vec4 Ambient::computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double** depthMap)
{
	return m.ambient * strength;
}

Vec4 Directional::computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double** depthMap)
{
	Vec4 d(0.0, 0.0, 0.0);
	Vec4 s(0.0, 0.0, 0.0);

	pin.normal = pin.normal.normalize();
	Vec4 lightDir = (direction * (-1.0)).normalize();
	double diff = max(pin.normal.dot(lightDir),0.0);
	
	Vec4 ref = Util::reflect(lightDir*(-1.0), pin.normal);
	double spec = pow(max(toEye.dot(ref), 0.0), 8);
	d = m.diffuse * diffuse * diff;
	s = m.specular * specular * spec;
	double bias = 0.0005;
	double shadow = shadowCalculation(pin.fragPosLightSpace, depthMap, pin.soneOverZ, bias);
	Vec4 res = (d + s)*(1.0-shadow);

	return res;
}

double Directional::shadowCalculation(Vec4& fragPosLightSpace, double** depthMap, double soneOverZ, double bias)
{
	int xIndex = (int)fragPosLightSpace.x;
	int yIndex = (int)fragPosLightSpace.y;
	double z = fragPosLightSpace.z * soneOverZ;
	if (xIndex >= 0 && xIndex < 1200 && yIndex >= 0 && yIndex < 900)
		double closest = depthMap[xIndex][yIndex];
	
	double shadow = 0.0;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (xIndex + x < 0 || xIndex + x >= 1200 || yIndex + y < 0 || yIndex + y >= 900)
				continue;
			double pcfDepth = depthMap[xIndex+x][yIndex+y];
			shadow += soneOverZ + bias < pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	return shadow;
	/*
	if (soneOverZ + bias > closest)
		return 0.0;
	return 1.0;
	*/
}

Vec4 PointLight::computeLightColor(VertexOut& pin, Vec4 toEye, Material& m, Vec4 texColor, double** depthMap)
{
	Vec4 d(0.0, 0.0, 0.0);
	Vec4 s(0.0, 0.0, 0.0);

	pin.normal = pin.normal.normalize();
	Vec4 lightDir = (position - pin.worldPos).normalize();
	double diff = max(pin.normal.dot(lightDir), 0.0);

	Vec4 ref = Util::reflect(lightDir*(-1.0), pin.normal);
	double spec = pow(max(toEye.dot(ref), 0.0), 8);
	d = m.diffuse * diffuse * diff;
	s = m.specular * specular * spec;

	double distance = (position - pin.worldPos).length();
	double attenuation = 1.0 / (constant + linear * distance + quadratic * (distance*distance));

	double bias = 0.0005;
	double shadow = shadowCalculation(pin.fragPosLightSpace, depthMap, pin.soneOverZ, bias);
	Vec4 res = (d + s)*(1.0 - shadow);

	return res;
}

double PointLight::shadowCalculation(Vec4& fragPosLightSpace, double** depthMap, double soneOverZ, double bias)
{
	int xIndex = (int)fragPosLightSpace.x;
	int yIndex = (int)fragPosLightSpace.y;
	double z = fragPosLightSpace.z * soneOverZ;
	if (xIndex >= 0 && xIndex < 1200 && yIndex >= 0 && yIndex < 900)
		double closest = depthMap[xIndex][yIndex];

	double shadow = 0.0;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (xIndex + x < 0 || xIndex + x >= 1000 || yIndex + y < 0 || yIndex + y >= 750)
				continue;
			double pcfDepth = depthMap[xIndex + x][yIndex + y];
			shadow += soneOverZ + bias < pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	return shadow;
}
