#include "Angle.hpp"

#include "MathUtil.hpp"

namespace Math
{
	
	Angle::Angle(double pitch, double yaw, double roll) :
		pitch(pitch), yaw(yaw), roll(roll)
	{
	}

	Angle::Angle() :
		pitch(0), yaw(0), roll(0)
	{
	}

	Angle::~Angle()
	{

	}

	Angle* Angle::ToDegree()
	{
		pitch = MathUtil::ToDeg(pitch);
		yaw = MathUtil::ToDeg(yaw);
		roll = MathUtil::ToDeg(roll);

		return this;
	}

	Angle* Angle::ToRadian()
	{
		pitch = MathUtil::ToRad(pitch);
		yaw = MathUtil::ToRad(yaw);
		roll = MathUtil::ToRad(roll);

		return this;
	}

	Angle Angle::GetDegree()
	{
		return Angle(MathUtil::ToDeg(pitch), MathUtil::ToDeg(yaw), MathUtil::ToDeg(roll));
	}

	Angle Angle::GetRadian()
	{
		return Angle(MathUtil::ToRad(pitch), MathUtil::ToRad(yaw), MathUtil::ToRad(roll));
	}

}