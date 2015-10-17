#ifndef MATH_H
#define MATH_H

#include "xmmintrin.h"

#define M_PI (3.14159265358979323846)
#define M_PI_F ((float)M_PI)

class matrix3x4_t
{

	float d[3][4];
public:

	float *operator[](int what)
	{
		return d[what];
	}

	const float *operator[](int what) const
	{
		return d[what];
	}
};

class Vector;
class QAngle;

extern void VectorAngles(const Vector &forward, QAngle &angles);
extern void MatrixPosition(const matrix3x4_t &matrix, Vector &pos);
extern void MatrixAngles(const matrix3x4_t &matrix, QAngle &angles);
extern void MatrixForward(const matrix3x4_t &matrix, Vector &forward);
extern void VectorTransform(const Vector &in1, const matrix3x4_t& in2, Vector &out);


#endif // MATH_H