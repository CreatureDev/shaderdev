#include "quadtree.h"
#include <stdlib.h>

struct quadtree *newqtree(int *ps, int *dm)
{
	struct quadtree *ret = calloc(1, sizeof(struct quadtree));
	ret->pos[0] = ps[0];
	ret->pos[1] = ps[1];
	ret->dim[0] = dm[0];
	ret->dim[1] = dm[1];
	return ret;
}

void freeqtree(struct quadtree *q)
{
	q->p = 0;
	if(q->c[0])
	{
		freeqtree(q->c[0]);
		freeqtree(q->c[1]);
		freeqtree(q->c[2]);
		freeqtree(q->c[3]);
	}
	free(q);
}

void splitqtree(struct quadtree *q)
{
	int ps[2], ds[2];
	if(q->c[0])
	{
		splitqtree(q->c[0]);
		splitqtree(q->c[1]);
		splitqtree(q->c[2]);
		splitqtree(q->c[3]);
		return;
	}
	ds[0] = q->dim[0] >> 1;
	ds[1] = q->dim[1] >> 1;
	ps[0] = q->pos[0] + (ds[0] >> 1);
	ps[1] = q->pos[1] + (ds[1] >> 1);
	q->c[0] = newqtree(ps, ds);
	ps[0] -= ds[0];
	q->c[1] = newqtree(ps, ds);
	ps[1] -= ds[1];
	q->c[2] = newqtree(ps, ds);
	ps[0] += ds[0];
	q->c[3] = newqtree(ps, ds);
}

