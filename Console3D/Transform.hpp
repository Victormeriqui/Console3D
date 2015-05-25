#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <stdint.h>

#include "Vector.hpp"
#include "Angle.hpp"
#include "Matrix.hpp"
#include "Orientation.hpp"

using namespace Math;

namespace Render
{
	class Transform
	{

	private:

		Vector translation;
		Orientation orientation;
		Vector scale;
		
	public:

		Transform();
		Transform(Vector trans, Orientation ori, Vector scl);
		~Transform();

		void ResetTransform();
		
		Matrix GetTransformationMatrix();
		Matrix GetReversedTransformationMatrix();
		
		Vector GetTranslation();
		void SetTranslation(const Vector* pos);
		void SetTranslation(double x, double y, double z);

		Orientation GetOrientation();
		void SetOrientation(const Orientation* orientation);
		
		//in degrees
		void RotatePitch(double theta);
		//in degrees
		void RotateYaw(double theta);
		//in degrees
		void RotateRoll(double theta);
		void Rotate(const Angle* ang);

		Vector GetScale();
		void SetScale(const Vector* vec);
		void SetScale(double x, double y, double z);

	};
}


#endif