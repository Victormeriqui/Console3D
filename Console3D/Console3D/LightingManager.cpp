#include "LightingManager.hpp"

#include "MathUtil.hpp"

namespace Render
{
	namespace Lighting
	{

		LightingManager::LightingManager() :
			directionallights_c(0), pointlights_c(0), spotlights_c(0), ambientlight(0)
		{			
		}

		LightingManager::~LightingManager()
		{
		}

		double LightingManager::SaturateLight(double lightamount)
		{
			return MathUtil::Clamp(lightamount, 0, 1);
		}

		double LightingManager::CalculateAllLightAmount(const Vector* vertexpos, const Vector* vertexnormal, bool model)
		{
			double lightamount = 0;

			lightamount += ambientlight;

			int i = 0;
						
			for (i = 0; i < directionallights_c; i++)			
				lightamount += directionallights[i].GetLightAmount(vertexpos, vertexnormal, model);

			for (i = 0; i < pointlights_c; i++)
				lightamount += pointlights[i].GetLightAmount(vertexpos, vertexnormal, model);

			for (i = 0; i < spotlights_c; i++)
				lightamount += spotlights[i].GetLightAmount(vertexpos, vertexnormal, model);
			
			return SaturateLight(lightamount);
		}

		void LightingManager::SetAmbientLight(double val)
		{
			ambientlight = val;
		}

		double LightingManager::GetAmbientLight()
		{
			return ambientlight;
		}

		void LightingManager::AddDirectionalLight(const DirectionalLight* light)
		{
			directionallights[directionallights_c++] = *light;
			lights[GetLightCount()-1] = (BaseLight*)(&directionallights[directionallights_c - 1]);
		}

		void LightingManager::AddPointLight(const PointLight* light)
		{
			pointlights[pointlights_c++] = *light;
			lights[GetLightCount()-1] = (BaseLight*)(&pointlights[pointlights_c - 1]);
		}

		void LightingManager::AddSpotLight(const SpotLight* light)
		{
			spotlights[spotlights_c++] = *light;
			lights[GetLightCount()-1] = (BaseLight*)(&spotlights[spotlights_c - 1]);
		}

		int LightingManager::GetDirectionalLightCount()
		{
			return directionallights_c;
		}

		int LightingManager::GetPointLightCount()
		{
			return pointlights_c;
		}

		int LightingManager::GetSpotLightCount()
		{
			return spotlights_c;
		}

		int LightingManager::GetLightCount()
		{
			return directionallights_c + pointlights_c + spotlights_c;
		}
		
		BaseLight* LightingManager::GetLight(int index)
		{
			return lights[index];
		}

		DirectionalLight* LightingManager::GetDirectionalLight(int index)
		{
			if (index < 0 || index >= directionallights_c)
				return nullptr;

			return &directionallights[index];
		}

		PointLight* LightingManager::GetPointLight(int index)
		{
			if (index < 0 || index >= pointlights_c)
				return nullptr;

			return &pointlights[index];
		}

		SpotLight* LightingManager::GetSpotLight(int index)
		{
			if (index < 0 || index >= spotlights_c)
				return nullptr;

			return &spotlights[index];
		}

		void LightingManager::RemoveDirectionalLight(int index)
		{
			if (index < 0 || index >= directionallights_c)
				return;

			for (int i = index; i < directionallights_c-1; i++)
				directionallights[i] = directionallights[i + 1];
		
			directionallights_c--;
		}

		void LightingManager::RemovePointLight(int index)
		{
			if (index < 0 || index >= pointlights_c)
				return;

			for (int i = index; i < directionallights_c - 1; i++)
				pointlights[i] = pointlights[i + 1];

			pointlights_c--;
		}

		void LightingManager::RemoveSpotLight(int index)
		{
			if (index < 0 || index >= spotlights_c)
				return;

			for (int i = index; i < spotlights_c - 1; i++)
				spotlights[i] = spotlights[i + 1];

			spotlights_c--;
		}

	}
}