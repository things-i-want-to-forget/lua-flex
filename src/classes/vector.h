#ifndef VECTOR_H
#define VECTOR_H


#include <cmath>
#include "drawing.h"
#include "math.h"
#include "xmmintrin.h"

#define VECTOR_FUNCTION static

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

		float out[4];

		_mm_store_ps(out, _mm_mul_ps(_mm_set_ps(x, y, z, 0), _mm_set_ps(o.x, o.y, o.z, 0)));

		return out[3] + out[2] + out[1];

	}
	inline Vector Transform(matrix3x4_t matrix)
	{
		Vector out;
		VectorTransform(*this, matrix, out);
		return out;
	}
	inline float Dot(const float *o) const
	{
		float out[4];

		_mm_store_ps(out, _mm_mul_ps(_mm_set_ps(x, y, z, 0), _mm_set_ps(o[0], o[1], o[2], 0)));

		return out[3] + out[2] + out[1];

	}
	inline void Rotate(const QAngle &angle)
	{
		VectorRotate(*this, angle);
	}
	inline Vector &Forward(void)
	{
		this->Normalize();
		*this *= M_PI_F;
		return *this;
	}
	inline QAngle &Angle(QAngle &a)
	{
		extern void VectorAngles(const Vector &_forward, QAngle &angles);
		VectorAngles(*this, a);
		return a;
	}
	inline float LengthSqr(void) const
	{

		float out[4];
		_mm_store_ps(out, _mm_mul_ps(_mm_set_ps(x,y,z,1), _mm_set_ps(x,y,z,1)));

		return out[3] + out[2] + out[1];

	}
	inline float Length(void) const
	{

		float out;

		_mm_store_ss(&out, _mm_sqrt_ss(_mm_set_ss(LengthSqr())));

		return out;

	}

	inline float Distance(Vector v2) const
	{
		return (v2 - *this).Length();
	}

	Vector operator-(const Vector &b)
	{
		float out[4];

		_mm_store_ps(out, _mm_sub_ps(_mm_set_ps(x, y, z, 0), _mm_set_ps(b.x, b.y, b.z, 0)));

		return Vector(out[3], out[2], out[1]);
	}

	Vector operator *(float amount)
	{
		return Vector(x * amount, y * amount, z * amount);
	}
	Vector operator /(float amount)
	{
		return Vector(x / amount, y / amount, z / amount);
	}

	Vector &operator *=(float amt)
	{
		*this = *this * amt;
		return *this;
	}

	Vector &operator /=(float amt)
	{
		*this = *this / amt;
		return *this;
	}
};

VECTOR_FUNCTION Vector operator+(const Vector &a, const Vector &b)
{
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

VECTOR_FUNCTION Vector operator-(const Vector &a, const Vector &b)
{
	float out[4];

	_mm_store_ps(out, _mm_sub_ps(_mm_set_ps(a.x, a.y, a.z, 0), _mm_set_ps(b.x, b.y, b.z, 0)));

	return Vector(out[3], out[2], out[1]);
}

VECTOR_FUNCTION Vector &operator+=(Vector &a, const Vector &b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

VECTOR_FUNCTION Vector &operator /=(Vector &v, float n)
{
	v.x /= n;
	v.y /= n;
	v.z /= n;
	return v;
}

VECTOR_FUNCTION Vector operator*(const Vector &v, const float &v2)
{
	return Vector(v.x * v2, v.y * v2, v.z * v2);
}

VECTOR_FUNCTION Vector operator/(const Vector &v, const float &v2)
{
	return Vector(v / v2);
}


VECTOR_FUNCTION bool operator==(const Vector &v, const Vector &v2)
{

	return _mm_movemask_ps(_mm_cmpneq_ps(_mm_set_ps(v.x, v.y, v.z, 0), _mm_set_ps(v2.x, v2.y, v2.z, 0))) == 0;

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
