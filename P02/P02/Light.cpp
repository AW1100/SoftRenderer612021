#include "Light.h"
#include <algorithm>
#include "Util.h"

Vec4 Ambient::computeLightColor(VertexOut& pin, Vec4 toEye, Material& m) const
{
	return m.ambient * strength * pin.color;
}

Vec4 Directional::computeLightColor(VertexOut& pin, Vec4 toEye, Material& m) const
{
	Vec4 d;
	Vec4 s;
	Vec4 l = (direction * (-1.0)).normalize();
	double diff = l.dot(pin.normal);
	if (diff > 0.0)
	{
		Vec4 ref = Util::reflect(direction, pin.normal);
		double spec = pow(max(ref.dot(toEye), 0.0), m.specular.w);
		d = m.diffuse * diffuse * diff;
		s = m.specular * specular * spec;
	}
	Vec4 res = pin.color * d + s;

	return res;
}