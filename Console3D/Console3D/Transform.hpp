#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

using namespace Math;

namespace Render
{
	class Transform
	{

	private:

		Vector translation;
		Quaternion rotation;
		Vector scale;
		
	public:

		Transform();
		Transform(const Vector* pos, const Quaternion* rot, const Vector* scl);
		~Transform();

		inline void ResetTransform();
		
		Matrix GetTransformationMatrix();
		
		Quaternion GetRotation();
		void SetRotation(const Quaternion* qua);
		void Rotate(const Quaternion* qua);
		void RotatePitch(double theta);
		void RotateYaw(double theta);
		void RotateRoll(double theta);

		Vector GetTranslation();
		void SetTranslation(const Vector* pos);
		void SetTranslation(double x, double y, double z);

		Vector GetScale();
		void SetScale(const Vector* vec);
		void SetScale(double x, double y, double z);

	};
}


#endif