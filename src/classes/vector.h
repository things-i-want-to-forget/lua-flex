#ifndef VECTOR_H
#define VECTOR_H


#include <cmath>
#include "drawing.h"

class Vector;
class QAngle;

extern void VectorRotate(Vector &vec, const QAngle &angle);

class Vector
{
public:
	Vector(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector() { }
	float x, y, z; 
	inline float &operator[](int which)
	{
		switch (which)
		{
		default:
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}
	inline const char *typestring(void) const { return "Vector"; }
	inline bool ToScreen(Vector &screen) const
	{
		return WorldToScreen(*this, screen);
	}

	inline Vector GetNormalized(void) {
		return Vector(*this / this->Length());
	}
	inline void Normalize(void)
	{
		*this = GetNormalized();
	}
	inline float Dot(const Vector &o) const
	{
		return x * o.x + y * o.y + z * o.z;
	}
	inline float Dot(const float *o) const
	{
		return x * o[0] + y * o[1] + z * o[2];
	}
	inline void Rotate(const QAngle &angle)
	{
		VectorRotate(*this, angle);
	}
	inline float LengthSqr(void) const
	{
		return y * y + z * z + x * x;
	}
	inline float Length(void) const
	{
		return sqrtf(LengthSqr());
	}

	Vector operator *(float amount)
	{
		return Vector(x * amount, y * amount, z * amount);
	}
	Vector operator /(float amount)
	{
		return Vector(*this * (1/amount));
	}

};

inline Vector operator+(const Vector &a, const Vector &b)
{
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline Vector operator-(const Vector &a, const Vector &b)
{
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline Vector &operator+=(Vector &a, const Vector &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}
inline Vector &operator *=(Vector &v, float n)
{
	v.x *= n;
	v.y *= n;
	v.z *= n;
	return v;
}

inline Vector &operator /=(Vector &v, float n)
{
	v.x /= n;
	v.y /= n;
	v.z /= n;
	return v;
}

inline Vector operator*(const Vector &v, const float &v2)
{
	return Vector(v.x * v2, v.y * v2, v.z * v2);
}

inline Vector operator/(const Vector &v, const float &v2)
{
	return Vector(v / v2);
}


inline bool operator==(const Vector &v, const Vector &v2)
{
	return v.x == v2.x && v.y == v2.y && v.z == v2.z;
}

class VectorAligned : public Vector
{
public:
	float w;

	VectorAligned &operator =(Vector &o)
	{
		x = o.x;
		y = o.y;
		z = o.z;
		return *this;
	}
};

#endif
