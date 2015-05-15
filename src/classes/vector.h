#ifndef VECTOR_H
#define VECTOR_H


#include <cmath>
#include "drawing.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#define M_PI_F (float)(M_PI)
#endif

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

	inline void Normalize(void)
	{
		float max;
		float _x, _y, _z;
		_x = sqrtf(x * x);
		_y = sqrtf(y * y);
		_z = sqrtf(z * z);
		if (_x > _y && _x > _z)
			max = _x;
		else if (_y > _z)
			max = _y;
		else
			max = _z;

		if (max != 0.00f)
		{
			z /= max;
			x /= max;
			y /= max;
		}
	}
	inline Vector &Forward(void)
	{
		this->Normalize();
		*this *= M_PI_F;
		return *this;
	}
	inline QAngle &Angle(QAngle &ang)
	{
		extern void VectorAngles(const Vector &forward, QAngle &angles);
		VectorAngles(*this, ang);
		return ang;
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
	inline Vector &operator *=(float n)
	{
		Vector &v = *this;
		v.x *= n;
		v.y *= n;
		v.z *= n;
		return v;
	}
};

inline Vector operator /(const Vector &_v, float n)
{
	Vector v = _v;
	v.x /= n;
	v.y /= n;
	v.z /= n;
	return v;
}
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
inline Vector operator*(const Vector &v, const float &v2)
{
	return Vector(v.x * v2, v.y * v2, v.z * v2);
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

#undef M_PI
#undef M_PI_F
#endif