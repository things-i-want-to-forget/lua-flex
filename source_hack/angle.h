#ifndef ANGLE_H
#define ANGLE_H
#include <cmath>

class Vector;

class QAngle
{
public:
	QAngle(float _p, float _y, float _r)
	{
		p = _p;
		y = _y;
		r = _r;
	}
	inline void Clamp(void);
	inline QAngle Difference(const QAngle &other) const;
	inline void Normalize(void);
	inline float Max(void) const;
	inline void Approach(const QAngle &other_ang, float amount);
	inline bool IsZero(void) const;
	inline void Zero(void);
	inline Vector &ToVector(Vector &v) const
	{
		extern Vector &AngleVectors(const QAngle &angles, Vector &v);
		return AngleVectors(*this, v);
	}
	inline float operator[](int which)
	{
		switch (which)
		{
		default:
		case 0:
			return p;
		case 1:
			return y;
		case 2:
			return r;
		}
	}
public:
	float p, y, r;
};


inline QAngle operator-(const QAngle &a, const QAngle &b)
{
	return QAngle(a.p - b.p, a.y - b.y, a.r - b.r);
}
inline QAngle operator+(const QAngle &a, const QAngle &b)
{
	return QAngle(a.p + b.p, a.y + b.y, a.r + b.r);
}
inline QAngle &operator+=(QAngle &a, const QAngle &b)
{
	a.p += b.p;
	a.y += b.y;
	a.r += b.r;
	return a;
}

inline QAngle operator-(const QAngle &ths)
{
	return QAngle(-ths.p, -ths.y, -ths.r);
}

inline QAngle &operator-=(QAngle &a, const QAngle &b)
{
	a.p -= b.p;
	a.y -= b.y;
	a.r -= b.r;
	return a;
}
inline QAngle operator*(const QAngle &a, float b)
{
	return QAngle(a.p * b, a.y * b, a.r * b);
}
inline QAngle operator/(const QAngle &a, float b)
{
	return QAngle(a.p / b, a.y / b, a.r / b);
}

inline QAngle QAngle::Difference(const QAngle &other) const
{
	QAngle diff = *this - other;
	diff.Normalize();
	return diff;
}

inline float _fmodf(float input, float modulo)
{
	while (input >= modulo)
		input -= modulo;
	while (input < 0)
		input += modulo;
	return input;
}

inline void QAngle::Normalize(void)
{
	p = _fmodf(p + 180, 360) - 180;
	if (p < -90) p = -90;
	if (p >= 90) p = 90;
	y = _fmodf(y + 180, 360) - 180;
	r = _fmodf(r + 180, 360) - 180;
}

inline void QAngle::Clamp(void)
{
	if (p < -89)
		p = -89;
	else if (p > 89)
		p = 89;
	y = _fmodf(y + 180, 360) - 180;
	r = 0;
}

inline bool QAngle::IsZero(void) const
{
	return p == 0 && y == 0 && r == 0;
}

inline void QAngle::Zero(void)
{
	p = 0;
	y = 0;
	r = 0;
}

inline float QAngle::Max(void) const
{
	if (p > r && p > y)
		return p;
	else if (r > y)
		return r;
	return y;
}
inline void QAngle::Approach(const QAngle &other_ang, float amount)
{
	QAngle diff = other_ang.Difference(*this);
	float max = diff.Max();
	if (max > amount)
		diff = diff / (max / amount);
	*this += diff;
}


#endif // ANGLE_H