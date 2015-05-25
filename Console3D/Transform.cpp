#include "Transform.hpp"

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Angle.hpp"
#include "Orientation.hpp"

using namespace Math;

namespace Render
{

	Transform::Transform()
	{
		ResetTransform();
	}

	Transform::Transform(Vector trans, Orientation ori, Vector scl)
	{
		translation = Vector();
		orientation = Orientation();
		scale = Vector(1, 1, 1);
	}

	Transform::~Transform()
	{

	}

	void Transform::ResetTransform()
	{
		translation = Vector();
		orientation = Orientation();
		scale = Vector(1, 1, 1);
	}
	
	Matrix Transform::GetTransformationMatrix()
	{
		Matrix translationm = *Matrix().SetTranslationMatrix(&translation);
		Matrix rotationm = *Matrix().SetQuaternionRotation(&orientation);
		Matrix scalem = *Matrix().SetScaleMatrix(&scale);

		return translationm * (rotationm * scalem);
	}

	Matrix Transform::GetReversedTransformationMatrix()
	{
		Matrix translationm = *Matrix().SetTranslationMatrix(&translation);
		Matrix rotationm = *Matrix().SetQuaternionRotation(&orientation);
		Matrix scalem = *Matrix().SetScaleMatrix(&scale);

		return rotationm * (translationm* scalem);
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

	Orientation Transform::GetOrientation()
	{
		return orientation;
	}

	void Transform::SetOrientation(const Orientation* ori)
	{
		orientation = *ori;
	}

	void Transform::RotatePitch(double theta)
	{
		orientation.RotatePitch(theta);
	}

	void Transform::RotateYaw(double theta)
	{
		orientation.RotateYaw(theta);
	}

	void Transform::RotateRoll(double theta)
	{
		orientation.RotateRoll(theta);
	}

	void Transform::Rotate(const Angle* ang)
	{
		orientation.Rotate(ang);
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