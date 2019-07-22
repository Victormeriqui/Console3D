#include "DirectionalLight.hpp"

#include "BaseLight.hpp"
#include "Vector.hpp"
#include "ShadowMap.hpp"
#include "Matrix.hpp"

namespace Render
{
	namespace Lighting
	{

		DirectionalLight::DirectionalLight() :
			direction(Vector()), BaseLight(1, false, 0)
		{
		}

		DirectionalLight::DirectionalLight(const Vector* direction, double intensity, bool castshadows) :
			direction(*direction), BaseLight(intensity, castshadows, 0)
		{
			if (castshadows)
			{
				pShadowMap = new ShadowMap(200, 200);
				lightprojmatrix = Matrix();
				
				lightprojmatrix.SetOrthographicProjectionMatrix(-5, 5, 5, -5, -5, 5);
				//lightprojmatrix.SetOrthographicProjectionMatrix(-40, 40, 40, -40, 0.1, 100);
				//lightprojmatrix.SetPerspectiveProjectionMatrix(100, 100, 0.1, 100, 90);
			}
		}

		DirectionalLight::~DirectionalLight()
		{
			if (castshadows)
			{
				if (pShadowMap != nullptr)
					delete pShadowMap;
			}
		}


		double DirectionalLight::GetLightAmount(const Vector* vertexpos, const Vector* vertexnormal, bool model)
		{
			double lightamount = model ? -direction.GetDotProduct(vertexnormal) : -direction.GetDotProduct(vertexnormal);;
			
			if (lightamount <= 0)
				return 0;

			return lightamount * intensity;
		}

		Matrix DirectionalLight::GetLightMatrix()
		{
			Matrix translationm = Matrix();
			translationm.SetTranslationMatrix(&(-direction ));
	
			Matrix scalem = Matrix();
			scalem.SetScaleMatrix(1, 1, 1);

			Vector dir = -direction;
			Vector yaxis = Vector(0, 1, 0);
			Vector right = yaxis.GetCrossProduct(&dir);
			right.Normalize();

			Vector up = dir.GetCrossProduct(&right);
			up.Normalize();

			Matrix rotationm = Matrix();
			rotationm.SetRotationMatrix(&right, &up, &dir);

			return lightprojmatrix *(rotationm);
		}


		Vector DirectionalLight::GetDirection()
		{
			return direction;
		}

		void DirectionalLight::SetDirection(const Vector* pos)
		{
			direction = *pos;
		}

	}
}