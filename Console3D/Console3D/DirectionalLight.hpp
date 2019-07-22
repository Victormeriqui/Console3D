#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "BaseLight.hpp"

#include "Attenuation.hpp"
#include "Vector.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		class DirectionalLight : public BaseLight
		{

		private:

			Vector direction;

		public:

			DirectionalLight();
			DirectionalLight(const Vector* direction, double intensity, bool castshadows);
			~DirectionalLight();

			virtual double GetLightAmount(const Vector * vertexpos, const Vector * vertexnormal, bool model);
			virtual Matrix GetLightMatrix();

			Vector GetDirection();
			void SetDirection(const Vector* pos);

		};

	}
}

#endif