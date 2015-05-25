#ifndef ORIENTATION_HPP
#define ORIENTATION_HPP

#include "Vector.hpp"
#include "Angle.hpp"

namespace Math
{
	class Orientation
	{

	private:
		
		Vector up;
		Vector forward;

	public:

		Orientation(const Vector* forward, const Vector* up);
		Orientation();
		~Orientation();

		//in degrees
		void RotatePitch(double theta);
		//in degrees
		void RotateYaw(double theta);
		//in degrees
		void RotateRoll(double theta);
		void Rotate(const Angle* ang);
		
		Vector GetUp();
		Vector GetForward();
		Vector GetLeft();

	};
	
}

#endif