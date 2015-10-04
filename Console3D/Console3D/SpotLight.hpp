#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "BaseLight.hpp"

#include "Attenuation.hpp"
#include "Vector.hpp"
#include "ShadowMap.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		class SpotLight : public BaseLight
		{

		private:

			Vector position;
			double range;
			Attenuation attenuation;

			Vector direction;
			double angle;

		public:

			SpotLight();
			SpotLight(const Vector* pos, const Vector* direction, double intensity, double range, double angle, bool castshadows);
			~SpotLight();

			virtual double GetLightAmount(const Vector * vertexpos, const Vector * vertexnormal);
			virtual Matrix GetLightMatrix();

			Vector GetPosition();
			void SetPosition(const Vector* pos);

			Vector GetDirection();
			void SetDirection(const Vector* dir);

			double GetRange();
			void SetRange(double val);
			
			double GetAngle();
			void SetAngle(double val);

			Attenuation GetAttenuation();
			void SetAttenuation(const Attenuation* att);


		};

	}
}

#endif