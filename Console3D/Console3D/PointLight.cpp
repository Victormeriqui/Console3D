#include "PointLight.hpp"

#include "Attenuation.hpp"
#include "BaseLight.hpp"
#include "Vector.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		PointLight::PointLight() :
			position(Vector()), range(1), BaseLight(1, false, 1)
		{
			attenuation.a = 1;
			attenuation.b = 1;
			attenuation.c = 1;
		}

		PointLight::PointLight(const Vector* pos, double intensity, double range, bool castshadows) :
			position(*pos), range(range), BaseLight(intensity, castshadows, 1)
		{
			attenuation.a = 1.2;
			attenuation.b = 1;
			attenuation.c = 1;

			if (castshadows)
			{
				pShadowMap = new ShadowMap(100, 100);
				lightprojmatrix = Matrix();
				lightprojmatrix.SetPerspectiveProjectionMatrix(100, 100, 0.1, 100, 90);
			}
		}

		PointLight::~PointLight()
		{
			if (castshadows)
			{
				if (pShadowMap != nullptr)
					delete pShadowMap;
			}
		}

		double PointLight::GetLightAmount(const Vector* vertexpos, const Vector* vertexnormal)
		{
			double lightamount = 0;

			Vector lightdir = (position - *vertexpos);
			double lightdist = lightdir.GetLength();

			if (lightdist >= range)
				return 0;
			
			lightdir.Normalize();
			
			double attval = attenuation.c + attenuation.b * lightdist + attenuation.a * lightdist * lightdist + 0.0001;

			double lightval = lightdir.GetDotProduct(vertexnormal);

			lightamount += (lightval * intensity) / attval;

			if (lightamount <= 0)
				return 0;

			return lightamount;
		}

		Matrix PointLight::GetLightMatrix()
		{
			return Matrix();
		}

		Vector PointLight::GetPosition()
		{
			return position;
		}

		void PointLight::SetPosition(const Vector* pos)
		{
			position = *pos;
		}

		double PointLight::GetRange()
		{
			return range;
		}

		void PointLight::SetRange(double val)
		{
			range = val;
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

		ShadowMap* PointLight::GetShadowMap()
		{
			return pShadowMap;
		}

	}
}