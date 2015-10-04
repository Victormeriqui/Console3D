#ifndef BASELIGHT_HPP
#define BASELIGHT_HPP

#include "Vector.hpp"
#include "ShadowMap.hpp"
#include "Matrix.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		class BaseLight
		{
		
		protected:
			
			int type;//0 = directional, 1 = point, 2 = spot

			double intensity;

			bool castshadows;
			ShadowMap* pShadowMap;

			Matrix lightprojmatrix;

		public:
			
			BaseLight()
			{
			}
			
			BaseLight(double intensity, bool castshadows, int type) :
				intensity(intensity), castshadows(castshadows), type(type)
			{				
			}

			~BaseLight() 
			{
			};

			virtual double GetLightAmount(const Vector* vertexpos, const Vector* vertexnormal) = 0;
			virtual Matrix GetLightMatrix() = 0;

			int GetType()
			{
				return type;
			}

			bool CastsShadows()
			{
				return castshadows;
			}

			ShadowMap* GetShadowMap()
			{
				return pShadowMap;
			}

			double GetIntensity()
			{
				return intensity;
			}

			void SetIntensity(double val)
			{
				intensity = val;
			}

		};

	}
}

#endif
