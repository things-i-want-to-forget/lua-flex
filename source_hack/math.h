#ifndef MATH_H
#define MATH_H

class Vector;
class QAngle;
typedef float matrix3x4_t[3][4];

extern void VectorAngles(const Vector &forward, QAngle &angles);
extern void MatrixPosition(const matrix3x4_t &matrix, Vector &pos);
extern void MatrixAngles(const matrix3x4_t &matrix, QAngle &angles);
extern void MatrixForward(const matrix3x4_t &matrix, Vector &forward);
extern void VectorTransform(const Vector &in1, const matrix3x4_t& in2, Vector &out);

#endif // MATH_H