#include "Transform.hpp"

#include "MathUtil.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

using namespace Math;

namespace Render
{

	Transform::Transform()
	{
		ResetTransform();
	}

	Transform::Transform(const Vector* pos, const Quaternion* rot, const Vector* scl)
	{
		translation = *pos;
		rotation = *rot;
		scale = *scl;
	}

	Transform::~Transform()
	{

	}

	void Transform::ResetTransform()
	{
		translation = Vector();
		rotation = Quaternion();
		scale = Vector(1, 1, 1);
	}
	
	Matrix Transform::GetTransformationMatrix()
	{
		Matrix translationm = Matrix();
		translationm.SetTranslationMatrix(&translation);
		
		Matrix rotationm = rotation.GetRotationMatrix();
		
		Matrix scalem = Matrix();
		scalem.SetScaleMatrix(&scale);

		return translationm * (rotationm * scalem);
	}

	Quaternion Transform::GetRotation()
	{
		return rotation;
	}

	void Transform::SetRotation(const Quaternion* qua)
	{
		rotation = *qua;
	}

	void Transform::Rotate(const Quaternion* qua)
	{
		rotation = (*qua) * rotation;
	}

	void Transform::RotatePitch(double theta)
	{
		rotation = Quaternion(&Vector(1, 0, 0), MathUtil::ToRad(theta)) * rotation;
	}
	
	void Transform::RotateYaw(double theta)
	{
		rotation = Quaternion(&Vector(0, 1, 0), MathUtil::ToRad(theta)) * rotation;
	}

	void Transform::RotateRoll(double theta)
	{
		rotation = Quaternion(&Vector(0, 0, 1), MathUtil::ToRad(theta)) * rotation;
	}

	Vector Transform::GetTranslation()
	{
		return translation;
	}

	void Transform::SetTranslation(const Vector* pos)
	{
		translation = *pos;
	}

	void Transform::SetTranslation(double x, double y, double z)
	{
		translation = Vector(x, y, z);
	}

	Vector Transform::GetScale()
	{
		return scale;
	}

	void Transform::SetScale(const Vector* vec)
	{
		scale = *vec;
	}

	void Transform::SetScale(double x, double y, double z)
	{
		scale = Vector(x, y, z);
	}

}