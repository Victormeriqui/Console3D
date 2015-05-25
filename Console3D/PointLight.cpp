#include "PointLight.hpp"

using namespace Math;

namespace Render
{

	PointLight::PointLight() :
		pos(Vector()), color(1), intensity(1), range(1)
	{
		attenuation.a = 1;
		attenuation.b = 1;
		attenuation.c = 1;
	}
	
	PointLight::PointLight(const Vector* pos, uint16_t color, double intensity, double range) : 
		pos(*pos), color(color), intensity(intensity), range(range)
	{
		attenuation.a = 1;
		attenuation.b = 1;
		attenuation.c = 1;
	}
	
	PointLight::~PointLight()
	{
	}

	Vector PointLight::GetPosition()
	{
		return pos;
	}

	void PointLight::SetPosition(const Vector* pos)
	{
		this->pos = *pos;
	}
	
	uint16_t PointLight::GetColor()
	{
		return color;
	}

	void PointLight::SetColor(uint16_t col)
	{
		color = col;
	}

	double PointLight::GetIntensity()
	{
		return intensity;
	}

	void PointLight::SetIntensity(double inten)
	{
		intensity = inten;
	}

	double PointLight::GetRange()
	{
		return range;
	}

	void PointLight::SetRange(double rang)
	{
		range = rang;
	}

	Attenuation PointLight::GetAttenuation()
	{
		return attenuation;
	}

	void PointLight::SetAttenuation(const Attenuation* att)
	{
		attenuation.a = att->a;
		attenuation.b = att->b;
		attenuation.c = att->c;
	}

}