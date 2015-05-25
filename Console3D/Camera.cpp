#include "Camera.hpp"

#include <math.h>
#include <stdint.h>

#include "Vector.hpp"
#include "Orientation.hpp"
#include "Matrix.hpp"

using namespace Math;

namespace Render
{

	Camera::Camera() :
		transform(Transform()), fov(90)
	{
	}

	Camera::Camera(double fov) :
		transform(Transform()), fov(fov)
	{
	}

	Camera::Camera(const Vector* pos, const Orientation* orientation, double fov) :
		transform(Transform(*pos, *orientation, Vector(1, 1, 1))), fov(fov)
	{
	}

	Camera::~Camera()
	{
	}

	Matrix Camera::GetCameraMatrix()
	{
		return transform.GetReversedTransformationMatrix();
	}

	double Camera::GetFOV()
	{
		return fov;
	}

	void Camera::SetPosition(const Vector* pos)
	{
		transform.SetTranslation(pos);
	}

	Vector Camera::GetPosition()
	{
		return transform.GetTranslation();
	}

	void Camera::MoveX(double amt)
	{		
		Vector left = transform.GetOrientation().GetLeft();
		Vector pos = transform.GetTranslation();
		
		transform.SetTranslation(pos.x + left.x*amt, pos.y + left.y*amt, pos.z + left.z*amt);
	}

	void Camera::MoveY(double amt)
	{
		Vector up = Vector(0, 1, 0);// transform.GetOrientation().GetUp();
		Vector pos = transform.GetTranslation();

		transform.SetTranslation(pos.x + up.x*amt, pos.y + up.y*amt, pos.z + up.z*amt);
	}

	void Camera::MoveZ(double amt)
	{
		Vector forward = transform.GetOrientation().GetForward();
		Vector pos = transform.GetTranslation();

		transform.SetTranslation(pos.x + forward.x*amt, pos.y + forward.y*amt, pos.z + forward.z*amt);
	}

	void Camera::Move(double xamt, double yamt, double zamt)
	{
		MoveX(xamt);
		MoveY(yamt);
		MoveZ(zamt);
	}

	void Camera::Move(const Vector* amt)
	{
		Move(amt->x, amt->y, amt->z);
	}

	Orientation Camera::GetOrientation()
	{
		return transform.GetOrientation();
	}

	void Camera::SetOrientation(const Orientation* ori)
	{
		transform.SetOrientation(ori);
	}

	void Camera::RotatePitch(double theta)
	{
		transform.RotatePitch(theta);
	}

	void Camera::RotateYaw(double theta)
	{
		transform.RotateYaw(theta);
	}

	void Camera::RotateRoll(double theta)
	{
		transform.RotateRoll(theta);
	}

	void Camera::Rotate(Angle ang)
	{
		transform.Rotate(&ang);
	}

}
