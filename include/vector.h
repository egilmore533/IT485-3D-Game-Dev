#ifndef __VECTOR_H__
#define __VECTOR_H__

#define PI				3.141589										/**< A macro that defines pi for use in variuous math stuffs */

#define ERIC_DEGTORAD	0.017453292519943295769236907684886f				/**< A macro that defines degrees to radians from DJ's MoGUL types file. */


typedef struct
{
	float x,y;
}Vect2D;

typedef struct
{
	float x,y,z;
}Vect3D;

typedef struct
{
	float x,y,z,w;
}Vect4D;

Vect2D vect2d(float x, float y);
Vect3D vect3d(float x, float y, float z);
Vect4D vect4d(float x, float y, float z, float w);

#define vect2d_add(dst,a,b)		(dst.x=a.x+b.x, dst.y=a.y+b.y)
#define vect3d_add(dst,a,b)		(dst.x=a.x+b.x, dst.y=a.y+b.y, dst.z=a.z+b.z)
#define vect4d_add(dst,a,b)		(dst.x=a.x+b.x, dst.y=a.y+b.y, dst.z=a.z+b.z, dst.w=a.w+b.w)

#endif
