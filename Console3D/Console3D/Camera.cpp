#include "Camera.hpp"

#include <math.h>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Transform.hpp"
#include "MathUtil.hpp"
#include "Quaternion.hpp"

using namespace Math;

namespace Render
{

	Camera::Camera() :
		transform(Transform()), fov(90)
	{
		projectionm = Matrix();
		projectionm.SetPerspectiveProjectionMatrix(0, 0, 0, 0, 0);
	}

	Camera::Camera(short width, short height, double znear, double zfar, double fov) :
		transform(Transform()), fov(fov)
	{
		projectionm = Matrix();
		projectionm.SetPerspectiveProjectionMatrix(width, height, znear, zfar, fov);		
		//projectionm.SetOrthographicProjectionMatrix(-5, 5, 5, -5, -5, 5);
	}

	Camera::~Camera()
	{
	}

	Matrix Camera::GetViewMatrix()
	{
		Matrix rotationm = transform.GetRotation().Conjugate()->GetRotationMatrix();
		Vector pos = -transform.GetTranslation();
		Matrix translationm = Matrix();
		translationm.SetTranslationMatrix(pos.x, pos.y, pos.z);

		return rotationm * translationm;
	}

	Matrix Camera::GetProjectionMatrix()
	{
		return projectionm;
	}

	Matrix Camera::GetProjectedViewMatrix()
	{
		Matrix rotationm = transform.GetRotation().GetConjugated().GetRotationMatrix();
		Vector pos = transform.GetTranslation() * -1.0;
		Matrix translationm = Matrix();
		translationm.SetTranslationMatrix(pos.x, pos.y, pos.z);
		
		return projectionm * rotationm *  translationm;
	}

	double Camera::GetFOV()
	{
		return fov;
	}

	Vector Camera::GetLookDirection()
	{
		return transform.GetRotation().GetForward();
	}

	void Camera::SetPosition(const Vector* pos)
	{
		transform.SetTranslation(pos);
	}

	Vector Camera::GetPosition()
	{
		//really need to watch out for this more, +30mins looking for this bug....
		Vector res = transform.GetTranslation();
		res.w = 1;
		
		return res;
	}

	Quaternion Camera::GetRotation()
	{
		return transform.GetRotation();
	}

	void Camera::SetRotation(const Quaternion* qua)
	{
		transform.SetRotation(qua);
	}

	void Camera::Rotate(const Quaternion* qua)
	{
		transform.Rotate(qua);
	}

	void Camera::RotatePitch(double theta)
	{
		transform.Rotate(&Quaternion(&transform.GetRotation().GetRight(), MathUtil::ToRad(theta)));
	}

	void Camera::RotateYaw(double theta)
	{
		transform.RotateYaw(theta);
	}

	void Camera::RotateRoll(double theta)
	{
		transform.RotateRoll(theta);
	}
	
	void Camera::Move(const Vector* dir, double val)
	{
		Vector curpos = transform.GetTranslation();
		curpos += (*dir) * val;

		transform.SetTranslation(&curpos);
	}
	
	void Camera::MoveX(double val)
	{
		Move(&transform.GetRotation().GetRight(), val);
	}

	void Camera::MoveY(double val)
	{
		Move(&transform.GetRotation().GetUp(), val);
	}

	void Camera::MoveZ(double val)
	{
		Move(&transform.GetRotation().GetForward(), val);
	}


}
