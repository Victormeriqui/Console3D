#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "BaseLight.hpp"

#include "Attenuation.hpp"
#include "Vector.hpp"
#include "ShadowMap.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		class PointLight : BaseLight
		{

		private:

			Vector position;	
			double range;
			Attenuation attenuation;

		public:

			PointLight();
			PointLight(const Vector* pos, double intensity, double range, bool castshadows);
			~PointLight();

			virtual double GetLightAmount(const Vector * vertexpos, const Vector * vertexnormal);
			virtual Matrix GetLightMatrix();

			Vector GetPosition();
			void SetPosition(const Vector* pos);

			double GetRange();
			void SetRange(double val);

			Attenuation GetAttenuation();
			void SetAttenuation(const Attenuation* att);

			ShadowMap* GetShadowMap();

		};

	}
}

#endif