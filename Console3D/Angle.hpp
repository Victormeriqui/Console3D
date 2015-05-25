#ifndef ANGLE_HPP
#define ANGLE_HPP

namespace Math
{
	class Angle
	{

	public:

		double pitch;
		double yaw;
		double roll;

		Angle(double pitch, double yaw, double roll);
		Angle();
		~Angle();

		Angle* ToDegree();
		Angle* ToRadian();
		Angle GetDegree();
		Angle GetRadian();

		Angle operator+ (Angle ang) const { return Angle(pitch + ang.pitch, yaw + ang.yaw, roll + ang.roll); }
		Angle operator- (Angle ang) const { return Angle(pitch - ang.pitch, yaw - ang.yaw, roll - ang.roll); }
		Angle operator/ (Angle ang) const { return Angle(pitch / ang.pitch, yaw / ang.yaw, roll / ang.roll); }
		Angle operator* (Angle ang) const { return Angle(pitch * ang.pitch, yaw * ang.yaw, roll * ang.roll); }
		Angle operator+= (Angle ang)
		{
			pitch += ang.pitch;
			yaw += ang.yaw;
			roll += ang.roll;

			return *this;
		}
		Angle operator-= (Angle ang)
		{
			pitch -= ang.pitch;
			yaw -= ang.yaw;
			roll -= ang.roll;

			return *this;
		}
		Angle operator/= (Angle ang)
		{
			pitch /= ang.pitch;
			yaw /= ang.yaw;
			roll /= ang.roll;

			return *this;
		}
		Angle operator*= (Angle ang)
		{
			pitch *= ang.pitch;
			yaw *= ang.yaw;
			roll *= ang.roll;

			return *this;
		}

		Angle operator+ (double n) const { return Angle(pitch + n, yaw + n, roll + n); }
		Angle operator- (double n) const { return Angle(pitch - n, yaw - n, roll - n); }
		Angle operator/ (double n) const { return Angle(pitch / n, yaw / n, roll / n); }
		Angle operator* (double n) const { return Angle(pitch * n, yaw * n, roll * n); }
		Angle operator+= (double n) { pitch += n; yaw += n; roll += n; return *this; }
		Angle operator-= (double n) { pitch -= n; yaw -= n; roll -= n; return *this; }
		Angle operator/= (double n) { pitch /= n; yaw /= n; roll /= n; return *this; }
		Angle operator*= (double n) { pitch *= n; yaw *= n; roll *= n; return *this; }

		Angle operator-() const { return Angle(-pitch, -yaw, -roll); }

	};
}

#endif