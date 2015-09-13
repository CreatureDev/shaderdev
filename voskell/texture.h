#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdlib.h>

#define TTYPE_1D 1
#define TTYPE_2D 2
#define TTYPE_3D 3

#define PTYPE_BYTE (sizeof(char))
#define PTYPE_SHORT (sizeof(short))
#define PTYPE_INT (sizeof(int))

#define PFORM_R 1
#define PFORM_RG 2
#define PFORM_RGB 3
#define PFORM_RGBA 4

#define PSIZE(x) ((x).ptype * (x).pform)

struct texture
{
	unsigned int ttype;
	unsigned int ptype;
	unsigned int pform;
	unsigned int w;
	unsigned int h;
	unsigned int d;
	char *pix;
};

#endif /* __TEXTURE_H__ */

