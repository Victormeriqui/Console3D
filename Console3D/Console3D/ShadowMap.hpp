#ifndef SHADOWMAP_HPP
#define SHADOWMAP_HPP

namespace Render
{

	class ShadowMap
	{

	private:

		double* pDepthMap;
		int width = 100;
		int height = 100;

	public:

		ShadowMap();
		ShadowMap(int width, int height);
		~ShadowMap();

		void SetDepthPoint(int x, int y, double depth);
		double GetDepthPoint(int x, int y);

		void ClearDepthMap();

		int GetWidth();
		int GetHeight();

	};

}

#endif