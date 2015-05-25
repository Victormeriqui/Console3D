#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Base
{
	namespace Time
	{
		static high_resolution_clock::time_point start;
		bool started = false;

		inline double GetCurTime()
		{
			if (!started)
			{
				start = high_resolution_clock::now();
				started = true;
			}

			high_resolution_clock::time_point now = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(now - start);
			return time_span.count();
		}

	}
}

#endif