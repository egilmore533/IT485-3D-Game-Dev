#include "vector.h"

Vect2D vect2d(float x, float y)
{
	Vect2D v = {x,y};
	return v;
}

Vect3D vect3d(float x, float y, float z)
{
	Vect3D v = {x,y,z};
	return v;
}

Vect4D vect4d(float x, float y, float z, float w)
{
	Vect4D v = {x,y,z,w};
	return v;
}
