#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <stdint.h>

#include "Vector.hpp"

using namespace Math;

namespace Render
{
	typedef struct Attenuation
	{
		double a;
		double b;
		double c;
	} Attenuation;

	class PointLight
	{
		
	private:

		Vector pos;
		double intensity;
		double range;
		Attenuation attenuation;
		uint16_t color;

	public:

		PointLight();
		PointLight(const Vector* pos, uint16_t color, double intensity, double range);
		~PointLight();

		Vector GetPosition();
		void SetPosition(const Vector* pos);

		uint16_t GetColor();
		void SetColor(uint16_t col);

		double GetIntensity();
		void SetIntensity(double intensity);

		double GetRange();
		void SetRange(double range);

		Attenuation GetAttenuation();
		void SetAttenuation(const Attenuation* att);

	};
}

#endif