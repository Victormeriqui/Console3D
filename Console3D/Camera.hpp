#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Transform.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Orientation.hpp"
#include "Angle.hpp"

using namespace Math;

namespace Render
{
	class Camera
	{

	private:

		Transform transform;
		double fov;

	public:

		Camera();
		Camera(double fov);
		Camera(const Vector* pos, const Orientation* orientation, double fov);
		~Camera();

		Matrix GetCameraMatrix();

		double GetFOV();

		Vector GetPosition();
		void SetPosition(const Vector* pos);

		void MoveX(double amt);
		void MoveY(double amt);
		void MoveZ(double amt);
		void Move(double xamt, double yamt, double zamt);
		void Move(const Vector* amt);

		Orientation GetOrientation();
		void SetOrientation(const Orientation* orientation);

		//in degrees
		void RotatePitch(double theta);
		//in degrees
		void RotateYaw(double theta);
		//in degrees
		void RotateRoll(double theta);
		void Rotate(Angle ang);

	};
}

#endif