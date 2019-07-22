#include "SpotLight.hpp"

#include <math.h>

#include "MathUtil.hpp"
#include "Attenuation.hpp"
#include "BaseLight.hpp"
#include "Vector.hpp"
#include "ShadowMap.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		SpotLight::SpotLight() :
			position(Vector()), range(1), BaseLight(1, false, 2)
		{
			attenuation.a = 1;
			attenuation.b = 1;
			attenuation.c = 1;
		}

		SpotLight::SpotLight(const Vector* pos, const Vector* direction, double intensity, double range, double angle, bool castshadows) :
			position(*pos), direction(*direction), range(range), angle(angle), BaseLight(intensity, castshadows, 2)
		{
			attenuation.a = 1;
			attenuation.b = 1;
			attenuation.c = 1;

			if (castshadows)
			{
				pShadowMap = new ShadowMap(200, 200);
				lightprojmatrix = Matrix();
				lightprojmatrix.SetPerspectiveProjectionMatrix(200, 200, 0.1, 100, 50);
			}
		}

		SpotLight::~SpotLight()
		{
			if (castshadows)
			{
				if (pShadowMap != nullptr)
					delete pShadowMap;
			}
		}

		double SpotLight::GetLightAmount(const Vector* vertexpos, const Vector* vertexnormal, bool model)
		{
			double lightamount = 0;
			
			Vector postolight = -(position - *vertexpos);
			postolight.Normalize();

			double spotfactor = postolight.GetDotProduct(&direction);

			double ang = MathUtil::ToDeg(acos(spotfactor));

			if (ang > angle)
				return 0;

			Vector lightdir = (position - *vertexpos);
			double lightdist = lightdir.GetLength();

			lightdir.Normalize();

			double attval = attenuation.c + attenuation.b * lightdist + attenuation.a * lightdist * lightdist + 0.0001;

			double lightval = lightdir.GetDotProduct(vertexnormal);

			lightamount += (lightval * intensity) / attval;

			if (lightamount <= 0)
				return 0;

			return lightamount;

			/*

			Vector lightdir = (*vertexpos - position);
			double lightdist = lightdir.GetLength3Comp();
			lightdir.Normalize();

			double inspotval = lightdir.GetDotProduct(&direction);

			if (inspotval > spotsize)
			{
				double attval = attenuation.c + attenuation.b * lightdist + attenuation.a * lightdist * lightdist + 0.0001;
				double lightval = -lightdir.GetDotProduct(vertexnormal);

				lightamount += (lightval * intensity) / attval;

				lightamount *= (1.0 - (1.0 - inspotval) / (1.0 - spotsize));
			}

			if (lightamount <= 0)
				return 0;*/

			return lightamount;
		}

		Matrix SpotLight::GetLightMatrix()
		{
			Matrix translationm = Matrix();
			translationm.SetTranslationMatrix(&(-position));

			Matrix scalem = Matrix();
			scalem.SetScaleMatrix(1, 1, 1);

			Vector yaxis = Vector(0, 1, 0);
			Vector right = yaxis.GetCrossProduct(&direction);
			right.Normalize();

			Vector up = direction.GetCrossProduct(&right);
			up.Normalize();

			Matrix rotationm = Matrix();
			rotationm.SetRotationMatrix(&right, &up, &direction);

			return lightprojmatrix * (rotationm * (translationm * scalem));
		}

		Vector SpotLight::GetPosition()
		{
			return position;
		}

		void SpotLight::SetPosition(const Vector* pos)
		{
			position = *pos;
		}

		Vector SpotLight::GetDirection()
		{
			return direction;
		}

		void SpotLight::SetDirection(const Vector* dir)
		{
			direction = *dir;
		}

		double SpotLight::GetRange()
		{
			return range;
		}

		void SpotLight::SetRange(double val)
		{
			range = val;
		}

		double SpotLight::GetAngle()
		{
			return angle;
		}

		void SpotLight::SetAngle(double val)
		{
			angle = val;
			lightprojmatrix.SetPerspectiveProjectionMatrix(200, 200, 0.1, 100, 50);
		}
		
		Attenuation SpotLight::GetAttenuation()
		{
			return attenuation;
		}

		void SpotLight::SetAttenuation(const Attenuation* att)
		{
			attenuation.a = att->a;
			attenuation.b = att->b;
			attenuation.c = att->c;
		}

		
	}
}