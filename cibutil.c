#include "texture.h"
#include "cibutil.h"
#include <stdio.h>
#include <stdlib.h>

struct texture *loadcib(const char *fnam)
{
	FILE *fl;
	struct texture *ret;
	unsigned long l;
	fl = fopen(fnam, "r");
	if(!fl)
	{
		printf("failed to open texture %s\n", fnam);
		return 0;
	}
	
	ret = (struct texture *) malloc(sizeof(struct texture));
	fread(ret, sizeof(unsigned int), 6, fl);
	l = PSIZE(*ret) * ret->w * ret->h * ret->d;
	ret->pix = (char *) malloc(l);
	fread(ret->pix, 1, l, fl);
	
	fclose(fl);
	return ret;
}

void freetexture(struct texture *ded)
{
	free(ded->pix);
	free(ded);
}

