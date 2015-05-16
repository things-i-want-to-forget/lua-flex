#include "angle.h"
#include "vector.h"
#include "math.h"
#include <cmath>

#define RAD2DEG(n) ((float)n * (float)(180.f / M_PI_F))
#define DEG2RAD(n) ((float)n * (float)(M_PI_F / 180.f))


void VectorTransform(const Vector &in1, const matrix3x4_t& in2, Vector &out);
void AngleMatrix(const QAngle& angles, matrix3x4_t& matrix);

void VectorRotate(Vector &vec, const QAngle &angle)
{
	Vector copy(vec.x, vec.y, vec.z);
	matrix3x4_t mat;
	AngleMatrix(angle, mat); 

	vec.x = copy.Dot(mat[0]);
	vec.y = copy.Dot(mat[1]);
	vec.z = copy.Dot(mat[2]);
}

void SinCos(float a, float *b, float *c)
{
	*b = sinf(a);
	*c = cosf(a);
}

void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.p), &sp, &cp);
	SinCos(DEG2RAD(angles.r), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;

	float crcy = cr*cy;
	float crsy = cr*sy;
	float srcy = sr*cy;
	float srsy = sr*sy;
	matrix[0][1] = sp*srcy - crsy;
	matrix[1][1] = sp*srsy + crcy;
	matrix[2][1] = sr*cp;

	matrix[0][2] = (sp*crcy + srsy);
	matrix[1][2] = (sp*crsy - srcy);
	matrix[2][2] = cr*cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}
Vector &AngleVectors(const QAngle &angles, Vector &v)
{
	float	sp, sy, cp, cy;
	sy = sinf(DEG2RAD(angles.y));
	cy = cosf(DEG2RAD(angles.y));
	sp = sinf(DEG2RAD(angles.p));
	cp = cosf(DEG2RAD(angles.p));

	v.x = cp*cy;
	v.y = cp*sy;
	v.z = -sp;
	v.Normalize();
	return v;
}

void VectorAngles(const Vector &_forward, QAngle &angles)
{
	float	tmp, yaw, pitch;

	Vector forward = _forward;
	forward.Forward();

	if (forward.y == 0 && forward.x == 0)
	{
		yaw = 0;
		if (forward.z > 0)
			pitch = -90;
		else
			pitch = 90;
	}
	else
	{
		yaw = RAD2DEG(atan2(forward.y, forward.x));
		/*
		if (yaw < -180)
			yaw += 360;
		else if (yaw >= 180)
			yaw -= 360;
			*/

		tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = RAD2DEG(atan2(-forward.z, tmp));
		/*
		if (pitch < -180)
			pitch += 360;
		else if (pitch >= 180)
			pitch -= 360;
			*/
		// fix yourself so you don't fix it a bunch of times
	}

	angles.p = pitch;
	angles.y = yaw;
	angles.r = 0;
}


void VectorTransform(const Vector &in1, const matrix3x4_t& in2, Vector &out)
{
	out.x = in1.Dot(in2[0]) + in2[0][3];
	out.y = in1.Dot(in2[1]) + in2[1][3];
	out.z = in1.Dot(in2[2]) + in2[2][3];
}


void MatrixPosition(const matrix3x4_t &matrix, Vector &pos)
{
	pos.x = matrix[0][3];
	pos.y = matrix[1][3];
	pos.z = matrix[2][3];
}

void MatrixForward(const matrix3x4_t &matrix, Vector &forward)
{
	forward.x = matrix[0][0];
	forward.y = matrix[1][0];
	forward.z = matrix[2][0];
}

void MatrixAngles(const matrix3x4_t &matrix, QAngle &angles)
{
	float forward[3];
	float left[3];
	float up[3];

	//
	// Extract the basis vectors from the matrix. Since we only need the Z
	// component of the up vector, we don't get X and Y.
	//
	forward[0] = matrix[0][0];
	forward[1] = matrix[1][0];
	forward[2] = matrix[2][0];
	left[0] = matrix[0][1];
	left[1] = matrix[1][1];
	left[2] = matrix[2][1];
	up[2] = matrix[2][2];

	float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

	// enough here to get angles?
	if (xyDist > 0.001f)
	{
		angles.y = RAD2DEG(atan2f(forward[1], forward[0]));

		angles.p = RAD2DEG(atan2f(-forward[2], xyDist));
		
		angles.r = RAD2DEG(atan2f(left[2], up[2]));
	}
	else	// forward is mostly Z, gimbal lock-
	{
		angles.y = RAD2DEG(atan2f(-left[0], left[1]));

		angles.p = RAD2DEG(atan2f(-forward[2], xyDist));

		angles.r = 0;
	}
}