#ifndef VECTOR_HPP
#define VECTOR_HPP


namespace Math
{
	class Quaternion;

	class Vector
	{

	public:

		double x;
		double y;
		double z;
		double w;

		Vector(double x, double y, double z);
		Vector(double x, double y, double z, double w);
		Vector();
		~Vector();

		double GetLength();
		Vector* Normalize();
		Vector GetNormalized() const;

		//Wless
		double GetLength3Comp();
		Vector* Normalize3Comp();

		double GetDistance(const Vector* vec);
		double GetDotProduct(const Vector* vec);
		Vector GetCrossProduct(const Vector* vec);
		Vector GetDirection(const Vector* vec);
		Vector GetMiddle(const Vector* vec);
	
		Vector GetRotated(const Vector* qaxis, double theta);
		Vector GetRotated(Quaternion* qua);

		void PerspectiveDivide();
		void PerspectiveMultiply();
		Vector Lerp(const Vector* dest, double val);

		Vector operator+ (Vector vec) const { return Vector(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
		Vector operator- (Vector vec) const { return Vector(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
		Vector operator/ (Vector vec) const { return Vector(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
		Vector operator* (Vector vec) const { return Vector(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
		Vector operator+= (Vector vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			w += vec.w;

			return *this;
		}
		Vector operator-= (Vector vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;

			return *this;
		}
		Vector operator/= (Vector vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			w /= vec.w;

			return *this;
		}
		Vector operator*= (Vector vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			w *= vec.w;

			return *this;
		}

		Vector operator+ (double n) const { return Vector(x + n, y + n, z + n, w + n); }
		Vector operator- (double n) const { return Vector(x - n, y - n, z - n, w - n); }
		Vector operator/ (double n) const { return Vector(x / n, y / n, z / n, w / n); }
		Vector operator* (double n) const { return Vector(x * n, y * n, z * n, w * n); }
		Vector operator+= (double n) { x += n; y += n; z += n; w += n; return *this; }
		Vector operator-= (double n) { x -= n; y -= n; z -= n; w -= n; return *this; }
		Vector operator/= (double n) { x /= n; y /= n; z /= n; w /= n; return *this; }
		Vector operator*= (double n) { x *= n; y *= n; z *= n; w *= n; return *this; }

		Vector operator-() const { return Vector(-x, -y, -z, -w); }

		bool operator==(Vector other) 
		{
			//avoid w, not used for comparision
			return (x == other.x && y == other.y && z == other.z);
		}

	};
}

#endif