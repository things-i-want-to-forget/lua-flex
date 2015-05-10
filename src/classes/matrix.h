#ifndef MATRIX_H
#define MATRIX_H

class VMatrix
{
public:
	float data[4][4];

public:
	float *operator[](int which)
	{
		return data[which];
	}
};

#endif // MATRIX_H