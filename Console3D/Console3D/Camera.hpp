#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Transform.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"

using namespace Math;

namespace Render
{
	class Camera
	{

	private:

		Matrix projectionm;
		Transform transform;
		double fov;

	public:

		Camera();
		Camera(short width, short height, double znear, double zfar, double fov);	
		~Camera();

		Matrix GetViewMatrix();
		Matrix GetProjectionMatrix();
		Matrix GetProjectedViewMatrix();

		double GetFOV();
		Vector GetLookDirection();

		Vector GetPosition();
		void SetPosition(const Vector* pos);
	
		void Move(const Vector* dir, double val);
		void MoveX(double val);
		void MoveY(double val);
		void MoveZ(double val);
		
		Quaternion GetRotation();
		void SetRotation(const Quaternion* qua);
		void Rotate(const Quaternion* qua);
		void RotatePitch(double theta);
		void RotateYaw(double theta);
		void RotateRoll(double theta);

				
	};
}

#endif