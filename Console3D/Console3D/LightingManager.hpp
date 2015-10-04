#ifndef LIGHTINGMANAGER_HPP
#define LIGHTINGMANAGER_HPP

#include "Vector.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

using namespace Math;

namespace Render
{
	namespace Lighting
	{

		class LightingManager
		{
		
		private:

			double ambientlight;

			BaseLight* lights[30];
			
			DirectionalLight directionallights[10];
			int directionallights_c;

			PointLight pointlights[10];
			int pointlights_c;

			SpotLight spotlights[1];
			int spotlights_c;
			
			double SaturateLight(double lightamount);

		public:

			LightingManager();
			~LightingManager();

			double CalculateAllLightAmount(const Vector* vertexpos, const Vector* vertexnormal);

			void SetAmbientLight(double val);
			double GetAmbientLight();

			void AddDirectionalLight(const DirectionalLight* light);
			void AddPointLight(const PointLight* light);
			void AddSpotLight(const SpotLight* light);

			int GetDirectionalLightCount();
			int GetPointLightCount();
			int GetSpotLightCount();

			int GetLightCount();
			BaseLight* GetLight(int index);

			DirectionalLight* GetDirectionalLight(int index);
			PointLight* GetPointLight(int index);
			SpotLight* GetSpotLight(int index);

			void RemoveDirectionalLight(int index);
			void RemovePointLight(int index);
			void RemoveSpotLight(int index);

		};

	}
}

#endif
