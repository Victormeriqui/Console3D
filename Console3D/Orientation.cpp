#include "Orientation.hpp"

#include "Vector.hpp"
#include "Angle.hpp"

namespace Math
{
	
	Orientation::Orientation(const Vector* forward, const Vector* up) :
		forward(*forward), up(*up)
	{
	}

	Orientation::Orientation() :
		forward(Vector(0, 0, 1)), up(Vector(0, 1, 0))
	{
	}

	Orientation::~Orientation()
	{
	}

	void Orientation::RotatePitch(double theta)
	{
		Vector left = GetLeft();

		forward.Rotate(&left, theta)->Normalize();
		up = *forward.GetCrossProduct(&left).Normalize();
	}
	
	void Orientation::RotateYaw(double theta)
	{
		forward.Rotate(&up, theta)->Normalize();
	}

	void Orientation::RotateRoll(double theta)
	{
		up.Rotate(&forward, theta)->Normalize();
	}

	void Orientation::Rotate(const Angle* ang)
	{
		if (ang->pitch != 0)
			RotatePitch(ang->pitch * 2);
		if (ang->yaw != 0)
			RotateYaw(ang->yaw * 2);
		if (ang->roll != 0)
			RotateRoll(ang->roll * 2);
	}

	Vector Orientation::GetUp()
	{
		return up;
	}

	Vector Orientation::GetForward()
	{
		return forward;
	}

	Vector Orientation::GetLeft()
	{
		return *(up.GetCrossProduct(&forward).Normalize());
	}
	
}